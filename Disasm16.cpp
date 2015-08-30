#include "Disasm16.hpp"
Disasm16::Disasm16(const string& fileName){
	if(loadBytes(fileName) == false)
		throw std::runtime_error("Unable to load file " + fileName);
}

Code Disasm16::getCode(){
	return std::move(loadCode());
}

Code Disasm16::loadCode(){
	Code code;
	while(i != bytes.size())
	{
		uint8_t possibleSPrefix = 0x00;
		
		if(checkPrefix(bytes[i])){
			possibleSPrefix = bytes[i];
			i++;
		}
		
		Opcode op = getOpcode(bytes[i]);
		Instruction instr = decode(op, possibleSPrefix);
		
		std::cout << instr << std::endl;
		
		//printf("%s %s %s\n", instr.opcode.ascii_str.c_str(), instr.dest_str.c_str(), instr.source_str.c_str());
		code.push_back(instr);

		i += op.fullLength;
	}
	return code;
}

bool Disasm16::checkPrefix(uint8_t argument)
{
	if(prefixes.count(argument) != 0){
		// Supprt only for segment prefixes;
		if( prefixes[argument] == NOT_SUPPORTED )
			throw std::runtime_error("Prefix " + to_string(argument) + " not supported");
		else
			return true;
	}
	else
		return false;
}

Opcode Disasm16::getOpcode(uint8_t byte)
{
	if(opcodeMap.count(bytes[i]) != 0)
		return opcodeMap[bytes[i]];
	else
		throw std::runtime_error("Unknown " + to_string(byte) + " op");
}

Instruction Disasm16::decode(const Opcode& op, uint8_t possiblePrefix)
{

	if(op.arg_format == MODRM)
	{
		return decodeModInstr(op, possiblePrefix);
	}
	if(op.arg_format == IMM)
	{
		throw std::runtime_error("Not Implemented yet :p");
		//printf("B\n");
	}
	if(op.arg_format == NONE)
		return Instruction(op, None, NONE_REG, NONE_REG, 0, 0, "", "");
	
	throw std::runtime_error("Unknown format");
}

Instruction Disasm16::decodeModInstr(const Opcode& opcode, uint8_t possiblePrefix){
	oprmmod_t opRmModInfo = decodeModRM(opcode, bytes[i + 1]);
	const RegMap* const regMap = selectRegMap(opRmModInfo);
	reg_type_t src_reg, dst_reg;
	string src_asc, dst_asc;
	if( opRmModInfo.mod_v == MEMORY_MODE )
	{
		// From register to memory.
		if(opRmModInfo.dir_v){
			dst_asc = regMap->at(opRmModInfo.reg_v).second, src_asc = IndirectAdressing[opRmModInfo.rmm_v].second;
			dst_reg = regMap->at(opRmModInfo.reg_v).first, src_reg = IndirectAdressing[opRmModInfo.rmm_v].first;
		}
		// From memory to register.
		if(opRmModInfo.dir_v == false){
			dst_asc = IndirectAdressing[opRmModInfo.rmm_v].second, src_asc = regMap->at(opRmModInfo.reg_v).second;
			dst_reg = IndirectAdressing[opRmModInfo.rmm_v].first, src_reg = regMap->at(opRmModInfo.reg_v).first;
		}
		
		return Instruction(opcode, Indirect, src_reg, dst_reg, bytes[i + 1], possiblePrefix, src_asc, dst_asc);
	}
	if( opRmModInfo.mod_v == REGISTER_MODE )
	{
		dst_asc = regMap->at(opRmModInfo.rmm_v << 3).second, src_asc = regMap->at(opRmModInfo.reg_v).second;
		dst_reg = regMap->at(opRmModInfo.rmm_v << 3).first, src_reg = regMap->at(opRmModInfo.reg_v).first;
		
		return Instruction(opcode, Indirect, src_reg, dst_reg, bytes[i + 1], 0, src_asc, dst_asc);		
	}
	throw std::runtime_error("Unknown MOD " + bin(opRmModInfo.mod_v) + " in MODRM byte");
}

Instruction decodeImmInstr(const Opcode& opcode, uint8_t possiblePrefix){
	
}

oprmmod_t Disasm16::decodeModRM(const Opcode& opcode, uint8_t modrm){
	return oprmmod_t{
			(uint8_t) (modrm  & 0b00111000), (uint8_t) (modrm  & 0b11000000),
			(uint8_t) (modrm  & 0b00000111), (uint8_t) (opcode.op & 0b00000010),
			(uint8_t) (opcode.op & 0b00000001)
	};
}

RegMap* Disasm16::selectRegMap(const oprmmod_t& oprmmod){
	if(oprmmod.siz_v)
		return &bigRegs;
	else
		return &smallRegs;
}

bool Disasm16::loadBytes(const string& fileName){
	ifstream file( fileName, std::ios::binary );
	if(file.is_open() == false)
		return false;
	bytes = 
		move( vector<uint8_t>( istreambuf_iterator<char>(file), 
				istreambuf_iterator<char>() ) );
	return true;
}

