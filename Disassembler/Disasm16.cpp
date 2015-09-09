#include "Disasm16.hpp"

namespace Engine
{

Disasm8086::Disasm8086(const std::string& fileName)
{
	if(readBytes(fileName) == false)
		throw std::runtime_error("Error while loading file " + fileName);
		
	decodeAll();			// Disassemble whole file
}

int Disasm8086::reconInstructionSize(const opdata_t& op) 
{
	if(op.encode == ONE_BYTE_ENC) 
		return 1;	     
	
	if(op.encode == MODRM_ENC || op.encode == MODRM_MEM_ENC || op.encode == MODRM_SEG_ENC)
		return reconDisplacementSize(decodeModRM(bytes[byteNo + 1]));
	
	if(op.encode == REG_IMM_ENC)
		return (op.src == IMM_IB)?2:3;

	if(op.encode == IMM_ENC || op.encode == JMP_ENC)
		return (op.dst == IMM_IB)?2:3;
		
	if(op.encode == MODRM_IMM_ENC)
	{
		int offsetSize = reconDisplacementSize(decodeModRM(bytes[byteNo+1]));	// Wielkość offsetu
		int immediateSize = (op.src == IMM_IB)?1:2;								// Immediate byte or word
		return offsetSize + immediateSize;
	}

	if(op.encode == AXAL_SEG_OFF_ENC)
		return 3;

	if(op.encode == RAW_SEG_RAW_OFF_ENC)
		return 5;

	if(op.encode == REG_REG_ENC)
		return 1;
		
	/* Group opcodez */
	if(op.encode == GRP_ENC)
	{
		modrm_type_t mod = decodeModRM(bytes[byteNo + 1]);
		grp_entry_t opval;
		try
		{
			opval = Groups.at(op.grp_index).at(mod.reg.to_ulong());
		}
		catch(...)
		{
			throw EncodeError();
		}
		if(opval.encode == MODRM_IMM_ENC)
		{
			int displacementSize = reconDisplacementSize(mod), argumentSize = 0;
			
			/* Non-standard encoding */
			if(opval.v0 != NONE && opval.v1 != NONE)
			{
				argumentSize = (opval.v1 == IMM_IB)?1:2;
				return displacementSize + argumentSize;
			}
			
			argumentSize = (op.src == IMM_IB)?1:2;
			return displacementSize + argumentSize;
		}
		if(opval.encode == MODRM_ARG_ONE || opval.encode == MODRM_ONE_ARG)
			return reconDisplacementSize(mod);
		
			
		/* Unknown group */
		/* Propably data, just skip */
		if(opval.encode == INVALID_ENC)
			throw EncodeError();
	}
	
	/* Invalid opcode, just skip */
	if(op.encode == INVALID_ENC)
		throw EncodeError();
	
	/* D3bug */
	throw std::logic_error(LOGERR("It should not happen, at byte " + std::to_string(byteNo)));
}

int Disasm8086::reconDisplacementSize(const modrm_type_t& mod) 
{
	/* Special (encoded in modrm.sel == 0b00) case [SWORD] */
	if( mod.sel == NO_DISPLACEMENT && 
	    mod.mod == SPECIAL_CASE_SWORD ) 
		return VAL_WORD_DISPLACEMENT;
		
	if(mod.sel == NO_DISPLACEMENT)	 return VAL_NO_DISPLACEMENT;
	if(mod.sel == REGISTER_TYPE)     return VAL_NO_DISPLACEMENT;
	if(mod.sel == BYTE_DISPLACEMENT) return VAL_BYTE_DISPLACEMENT;
	if(mod.sel == WORD_DISPLACEMENT) return VAL_WORD_DISPLACEMENT;
	
	/* D3bug */
	throw std::logic_error(LOGERR("It should not happen"));
}

void Disasm8086::decodeAll()
{
    while(allParsed() == false)
	{
		int PC = byteNo;						// Program counter
		prefix_pack_t fixpack = grabPrefixes(); // Grab prefix bytes
		opdata_t op = opmap[bytes[byteNo]];		// Fetch opcode info from map
		
		instruction_t instr;				
		int bytesToSkip;
		
		try
		{
			bytesToSkip = reconInstructionSize(op);
			
			switch(op.encode)
			{
				case REG_REG_ENC:
				case ONE_BYTE_ENC:
					instr = instruction_t(op);
					break;
				case MODRM_ENC:
					instr = decodeModRMInstr(op);
					break;
				case GRP_ENC:
					instr = decodeGroupInstr(op);
					break;
				case REG_IMM_ENC:
					instr = decodeRegImmInstr(op);
					break;
				case IMM_ENC:
					instr = decodeImmInstr(op);
					break;
				case MODRM_IMM_ENC:
					instr = decodeModRMImmInstr(op);
					break;
				case JMP_ENC:
					instr = decodeJmpInstr(op);
					break;
				case MODRM_SEG_ENC:
					instr = decodeModSegInstr(op);
					break;
				case AXAL_SEG_OFF_ENC:
					instr = decodeAxAxSegInstr(op);
					break;
				case MODRM_MEM_ENC:
					instr = decodeModRMemInstr(op);
					break;
				case RAW_SEG_RAW_OFF_ENC:
					instr = decodeRawJmpInstr(op);
					break;
				default:
					/* D3bug */
					throw std::runtime_error("Unknown encoding");	
			}
		}
		
		catch(EncodeError)
		{
			instr = dataByte();
			code.push_back(instr);
			std::cout << instr << std::endl;
			if(byteNo < bytes.size()) 
			{
				byteNo++; 
				continue;
			}
			if(byteNo >= bytes.size()) return;
		}
				
		instr.fixpack = fixpack;
		instr.position = PC;
		code.push_back(instr);				
		
		std::cout <<std::hex << (unsigned) byteNo << ":"<< instr << std::endl;
		byteNo += bytesToSkip;
	}
}

// <prefixy> <opcode> <modrm> (displacement)
instruction_t Disasm8086::decodeModRMInstr(opdata_t& op) throw(EncodeError)
{
	
	modrm_type_t modrm = decodeModRM(bytes[byteNo + 1]);		// decode modrm byte
	int displacementSize = reconDisplacementSize(modrm);
	
	if( (byteNo + displacementSize) > bytes.size() )			// No enough space for displacement bytes
		throw EncodeError();									// Propably data
	
	RegisterMap* regs = selectRegisterMapBySize(op);	
	instr_payload possibleOffset;								// placeholder for displacement
	
	try
	{
		if(modrm.sel == REGISTER_TYPE)							// Reg, Reg encoding
		{
			op.src = regs->at(modrm.reg.to_ulong()), op.dst = regs->at(modrm.mod.to_ulong());
			return instruction_t(op);
		}
	
		if(op.dir() == FROM_REGISTER)								
			op.dst = (ModRmMap.at(modrm.sel.to_ulong()).at(modrm.mod.to_ulong())), op.src = regs->at(modrm.reg.to_ulong());
	
		if(op.dir() == TO_REGISTER)
			op.src = (ModRmMap.at(modrm.sel.to_ulong()).at(modrm.mod.to_ulong())), op.dst = regs->at(modrm.reg.to_ulong());
		
		if(displacementSize == VAL_NO_DISPLACEMENT)					
			return instruction_t(op, VAL_NO_DISPLACEMENT);
	
		if(displacementSize == VAL_BYTE_DISPLACEMENT)
			possibleOffset.sbyte = bytes[byteNo + 2];
		
		if(displacementSize == VAL_WORD_DISPLACEMENT)
			possibleOffset.sword = bytes[byteNo + 3] << 8 | bytes[byteNo + 2];

		return instruction_t(op, displacementSize, instr_payload(), possibleOffset);
	}
	
	catch(...)
	{
		throw EncodeError();
	}
}
// <grp-opcode> ( depend on type bytes )
instruction_t Disasm8086::decodeGroupInstr(opdata_t& op) throw(EncodeError)
{
	modrm_type_t mod = decodeModRM(bytes[byteNo + 1]);
	grp_entry_t opval;
	
	try
	{
		opval = Groups.at(op.grp_index).at(mod.reg.to_ulong());	
	}
	catch(...)
	{
		throw EncodeError();
	}
	
	/* Non-standard encoding */
	if(opval.v0 != NONE)
		op.dst = opval.v0, op.src = opval.v1;
		
	op.encode = opval.encode;
	op.name   = opval.name;
	
	if(opval.encode == MODRM_IMM_ENC)
		return decodeModRMImmInstr(op);
		
	if(opval.encode == MODRM_ONE_ARG || opval.encode == MODRM_ARG_ONE)
		return decodeOneArgInstr(op);

}

// <register> <immediate>
instruction_t Disasm8086::decodeRegImmInstr(opdata_t& op) 
{
	instr_payload mainPayload;
	
	if(isSmallReg(op.src))
		mainPayload.sbyte = bytes[byteNo + 1];
	else
		mainPayload.sword = bytes[byteNo + 2] << 8 | bytes[byteNo + 1];
		
	return instruction_t(op, VAL_NO_DISPLACEMENT, mainPayload);
}

// <immediate> 
instruction_t Disasm8086::decodeImmInstr(opdata_t& op)
{
	instr_payload mainPayload;
	
	if(isImmediateByte(op.dst))
		mainPayload.sbyte = bytes[byteNo + 1];
	
	if(isImmediateWord(op.dst))
		mainPayload.sword = bytes[byteNo + 2] << 8 | bytes[byteNo + 1];
		
	return instruction_t(op, VAL_NO_DISPLACEMENT, mainPayload);
}

// <modrm> <immediate> 
instruction_t Disasm8086::decodeModRMImmInstr(opdata_t& op) throw(EncodeError)
{
	instr_payload payload, offset;
	modrm_type_t modrm = decodeModRM(bytes[byteNo + 1]);
	int displacementSize = reconDisplacementSize(modrm);
	RegisterMap* regs = (op.size())?&Reg16Map:&Reg8Map;
	
	try
	{
		if(modrm.sel == REGISTER_TYPE)
			op.dst = regs->at(modrm.mod.to_ulong());
		else 
		{
			op.dst = ModRmMap.at(modrm.sel.to_ulong()).at(modrm.mod.to_ulong());
		}
	}
	catch(...)
	{
		throw EncodeError();
	}
	
	if(displacementSize == VAL_BYTE_DISPLACEMENT)
		offset.sbyte = bytes[byteNo + 2];
	if(displacementSize == VAL_WORD_DISPLACEMENT)
		offset.sword = bytes[byteNo + 3] << 8 | bytes[byteNo + 2];
	
	if(op.src == IMM_IB)
		payload.sbyte = bytes[byteNo + (displacementSize-1) + 1];

	if(op.src == IMM_IV)
		payload.sword = bytes[byteNo + (displacementSize-1) + 2] << 8 | bytes[byteNo + (displacementSize-1) + 1];

	return instruction_t(op, displacementSize, payload, offset);
}

// <immediate> (need to calculate real offset)
instruction_t Disasm8086::decodeJmpInstr(opdata_t& op)
{
	instr_payload offst, roffst;
	
	if(op.dst == IMM_IB)
	{
		offst.sbyte = bytes[byteNo + 1];
		// Real position in code
		roffst.ubyte = (offst.sbyte + 0x2) + byteNo;
	}
	if(op.dst == IMM_IV)
	{
		offst.sword = bytes[byteNo + 2] << 8 | bytes[byteNo + 1];
		roffst.uword = (offst.sword + 0x3) + byteNo;
	}
	return instruction_t(op, VAL_NO_DISPLACEMENT, offst, roffst);
}

// <modrm> rev ok
instruction_t Disasm8086::decodeOneArgInstr(opdata_t& op) throw(EncodeError)
{
	RegisterMap* regs = (op.size())?&Reg16Map:&Reg8Map;
	modrm_type_t modrm = decodeModRM(bytes[byteNo + 1]);
	instr_payload offzt;
	// special case
	try
	{
		if(modrm.sel.to_ulong() == REGISTER_TYPE)
		{
			op.dst = regs->at(modrm.mod.to_ulong());
			return instruction_t(op, VAL_NO_DISPLACEMENT);
		}
	}
	catch(...)
	{
		throw EncodeError();
	}
	
	int displacementSize = reconDisplacementSize(modrm);

	if(displacementSize == VAL_BYTE_DISPLACEMENT)
		offzt.sbyte = bytes[byteNo + 2];
		
	if(displacementSize == VAL_WORD_DISPLACEMENT)
		offzt.sword = bytes[byteNo + 3] << 8 | bytes[byteNo + 2];

	try
	{
		op.dst = ModRmMap.at(modrm.sel.to_ulong()).at(modrm.mod.to_ulong());
	}
	catch(...)
	{
		throw EncodeError();
	}
	
	return instruction_t(op, displacementSize, instr_payload(), offzt);
}

// <modrm> ale seg
instruction_t Disasm8086::decodeModSegInstr(opdata_t& op) throw(EncodeError)
{
	modrm_type_t modrm = decodeModRM(bytes[byteNo + 1]);
	RegisterMap* regs = &Reg16Map, *segs = &segmap;
	instr_payload possibleOffset;								
	int displacementSize = reconDisplacementSize(modrm);	
	
	try
	{
		if(modrm.sel == REGISTER_TYPE)
		{
			if(op.dir() == TO_REGISTER)	  // From gpr to seg
				op.dst = segs->at(modrm.reg.to_ulong()), op.src = regs->at(modrm.mod.to_ulong());
			if(op.dir() == FROM_REGISTER) // From seg to gpr
				op.dst = regs->at(modrm.mod.to_ulong()), op.src = segs->at(modrm.reg.to_ulong());
			return instruction_t(op);
		}
	
	op.dst = ModRmMap.at(modrm.sel.to_ulong()).at(modrm.mod.to_ulong());
	op.src = segs->at(modrm.reg.to_ulong());
	}
	catch(...)
	{
		throw EncodeError();
	}
	
	if(displacementSize == VAL_BYTE_DISPLACEMENT)
		possibleOffset.sbyte = bytes[byteNo + 2];
	
	if(displacementSize == VAL_WORD_DISPLACEMENT)
		possibleOffset.sword = bytes[byteNo + 3] << 8 | bytes[byteNo + 2];
	
	return instruction_t(op, displacementSize, instr_payload(), possibleOffset);
}

instruction_t Disasm8086::decodeAxAxSegInstr(opdata_t& op)
{
	RegisterMap* regs = (op.size())?&Reg16Map:&Reg8Map;
	instr_payload offzt;
	offzt.sword = bytes[byteNo + 2] << 8 | bytes[byteNo + 1];
	return instruction_t(op, VAL_WORD_DISPLACEMENT, instr_payload(), offzt);
}

// LEA case
instruction_t Disasm8086::decodeModRMemInstr(opdata_t& op) throw(EncodeError)
{
	modrm_type_t modrm = decodeModRM(bytes[byteNo + 1]);
	int displacementSize = reconDisplacementSize(modrm);
	instr_payload displacement;
	
	try
	{
		op.dst = Reg16Map.at(modrm.reg.to_ulong()), op.src = ModRmMap.at(modrm.sel.to_ulong()).at(modrm.mod.to_ulong());
	}
	catch(...)
	{
		throw EncodeError();
	}
	
	if(displacementSize == VAL_NO_DISPLACEMENT)
		return instruction_t(op, VAL_NO_DISPLACEMENT);
	
	if(displacementSize == VAL_BYTE_DISPLACEMENT)
		displacement.sbyte = bytes[byteNo + 2];
	
	if(displacementSize == VAL_WORD_DISPLACEMENT)
		displacement.sword = bytes[byteNo + 3] << 8 | bytes[byteNo + 2];
	
	return instruction_t(op, displacementSize, instr_payload(), displacement);
}

instruction_t Disasm8086::decodeRawJmpInstr(opdata_t& op)
{
	instr_payload p_off, p_seg;
	p_seg.uword = bytes[byteNo + 4] << 8 | bytes[byteNo + 3];
	p_off.uword = bytes[byteNo + 2] << 8 | bytes[byteNo + 1];
	return instruction_t(op, VAL_NO_DISPLACEMENT, instr_payload(), p_off, p_seg);
}

instruction_t Disasm8086::dataByte()
{
	std::array<char, 16> buffer;
	instr_payload byte;
	byte.sbyte = bytes[byteNo];
	snprintf(buffer.data(), 16, ".byte %x", bytes[byteNo]);
	opdata_t op(0xff, buffer.data(), INVALID, INVALID, INVALID_ENC);
	return instruction_t(op, VAL_NO_DISPLACEMENT, byte);
}

bool Disasm8086::readBytes(const std::string& fileName)
{
	std::ifstream file( fileName, std::ios::binary );
	
    if(file.is_open() == false)
        return false;
        
    bytes = 
        std::move( std::vector<uint8_t>( std::istreambuf_iterator<char>(file), 
										 std::istreambuf_iterator<char>() ) );
	return true;
}

inline bool Disasm8086::allParsed() const 
{
	return ( byteNo == bytes.size() );
}

modrm_type_t Disasm8086::decodeModRM(uint8_t modbyte) const
{
	return modrm_type_t{modbyte >> 6 ,modbyte , modbyte >> 3};
}

prefix_pack_t Disasm8086::grabPrefixes()
{
	prefix_type_t fixType;
	prefix_pack_t fixPack;
	while( (fixType = prefixType(bytes[byteNo])) != FIX_NONE )
	{
		opdata_t opdata = opmap[bytes[byteNo]];
		switch(fixType)
		{
			case FIX_LOCK:
				fixPack.lck_fix = opdata.dst;
				break;
			case FIX_REP:
				fixPack.rep_fix = opdata.dst;
				break;
			case FIX_REG:
				fixPack.reg_fix = opdata.dst;
				break;
			case FIX_NONE:
				/* Shut up gcc -Wall */
				break;
			default:
				throw std::logic_error(LOGERR("It should not happen"));		
		}
		byteNo++;
	}
	return fixPack;
}

inline RegisterMap* const Disasm8086::selectRegisterMapBySize(const opdata_t& op)
{
	return (op.size())?&Reg16Map:&Reg8Map;
}

} // Engine
