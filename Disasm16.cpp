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
    if(op.arg_format == MODRM_IMM)
    {
        return decodeMemImmInstr(op, possiblePrefix);
    }
    if(op.arg_format == IMM)
    {
        return decodeImmInstr(op, possiblePrefix);
    }
    if(op.arg_format == JMP_REL)
    {
        return decodeJmpInstr(op, possiblePrefix);
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
        if(opcode.fullLength == 2)
        {
            dst_asc = regMap->at(opRmModInfo.rmm_v << 3).second, src_asc = regMap->at(opRmModInfo.reg_v).second;
            dst_reg = regMap->at(opRmModInfo.rmm_v << 3).first, src_reg = regMap->at(opRmModInfo.reg_v).first;
        
            return Instruction(opcode, Indirect, src_reg, dst_reg, bytes[i + 1], 0, src_asc, dst_asc);      
        }
        // Op/RMMOD/Imm
        if(opcode.fullLength == 4)
        {
            throw std::runtime_error("Hmm?");
        }
    }
    throw std::runtime_error("Unknown MOD " + bin(opRmModInfo.mod_v) + " in MODRM byte");
}

Instruction Disasm16::decodeImmInstr(const Opcode& opcode, uint8_t possiblePrefix){
    // Check is in imm-set.
    if(immedMap.count(opcode.op) == 0)
        throw std::runtime_error("Opcode op " + to_string(opcode.op) + " not in imm but MODRM");
    uint16_t argument = 0;
    imm_val_t immreg = immedMap[opcode.op];

    // Op/16-bit value
    if(opcode.fullLength == 3){
        argument = bytes[i + 2] << 8 | bytes[i + 1];        // Swap bytes.      
    }
    // Op/8-bit value
    if(opcode.fullLength == 2){
        argument = bytes[i + 1];
    }
    
    return Instruction(opcode, RegisterAdressing, immreg.src, immreg.dst, argument, possiblePrefix, "", "");
    
}

Instruction Disasm16::decodeJmpInstr(const Opcode& opcode, uint8_t possiblePrefix){
    uint16_t absoluteJump = 0;
    if(opcode.imm_size == 1)
    {
        char relativeJump;
        relativeJump = bytes[i + 1];
        absoluteJump = i + opcode.fullLength - abs(relativeJump);
    }
    //Instruction(Opcode op, addr_type_t addrt, reg_type_t so, reg_type_t des, uint16_t arg, uint8_t spc, std::string sstr, std::string dstr)
    if(opcode.imm_size == 2)
    {
        int16_t relativeJump;
        relativeJump = bytes[i + 2] << 8 | bytes[i + 1];
        absoluteJump = i + opcode.fullLength - abs(relativeJump);
    }
    return Instruction(opcode, None, NONE_REG, NONE_REG, absoluteJump, 0, "", "");
}

Instruction Disasm16::decodeMemImmInstr(const Opcode& opcode, uint8_t possiblePrefix){
    oprmmod_t opRmModInfo = decodeModRM(opcode, bytes[i + 1]);
    reg_type_t dst_reg;
    uint16_t val;
    string dst_asc;
    dst_asc = IndirectAdressing[opRmModInfo.rmm_v].second, dst_reg = IndirectAdressing[opRmModInfo.rmm_v].first;
    val = bytes[i + 3] << 8 | bytes[i + 2];
    return Instruction(opcode, IndirMemory, NONE_REG, dst_reg, val, possiblePrefix, "", dst_asc);
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
