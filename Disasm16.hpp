#ifndef DISASM16
#define DISASM16

#include <map>
#include <vector>
#include <string>
#include <limits>
#include <fstream>
#include <iostream>

using namespace std;
#define REGISTER_MODE 0b11000000
#define MEMORY_MODE 0b000

enum addr_type_t { Indirect, RegisterAdressing , IndirMemory,  Utility, None };

enum reg_type_t {   
     AX, CX, DX, BX, SP, DP, SI, DI, AL, CL, DL, BL, AH, CH, DH, BH, BP, 
     ADDR_BX_SI, ADDR_BX_DI, ADDR_BP_SI, ADDR_BP_DI, ADDR_SI, ADDR_DI, ADDR_SWORD, ADDR_BX, IMMVAL, NONE_REG
};

enum format_type_t {
    MODRM, MODRM_IMM, IMM, JMP_REL,  NONE
};

enum override_type_t {
    NOT_SUPPORTED,
    CS, SS, DS, ES, FS, GS
};

struct Opcode
{
    Opcode()
    {
    }
    Opcode(uint8_t _op, int _fL, int _c, string _as, format_type_t _frmt, size_t imms)
        : op(_op), fullLength(_fL), cycles(_c), ascii_str(_as), arg_format(_frmt), imm_size(imms) 
    {}
    
    uint8_t op;                     // Opcode
    int fullLength;                 // Full length of opcode + modrm/imm
    int cycles;                     // Cycles
    std::string ascii_str;          // Ascii string name
    format_type_t arg_format;       // Format of opcode modRM
    int imm_size;                   // If imm, size of imm arguments.
};

struct Instruction
{
    Instruction()
    {
    }
    Instruction(Opcode op, addr_type_t addrt, reg_type_t so, reg_type_t des, uint16_t arg, uint8_t spc, std::string sstr, std::string dstr)
        : opcode(op), addr_type(addrt), source(so), dest(des), argument(arg), special(spc), source_str(sstr), dest_str(dstr)
    {
    }
    Opcode opcode;                      // Opcode
    addr_type_t addr_type;              // Addressing type in instruction 
    reg_type_t source, dest;            // Source and dest options
    uint16_t argument;                  // Argument
    uint8_t special;                    // Maybe prefix
    std::string source_str, dest_str;   // Source string and dest string
};

struct imm_val_t
{
    reg_type_t src, dst;
};

static std::string transReg(uint8_t prefx)
{
    switch(prefx)
    {
        case 0x2e:
            return "CS";
        case 0x36:
            return "SS";
        case 0x3e:
            return "DS";
        case 0x26:
            return "ES";
        case 0x64:
            return "FS";
        case 0x65:
            return "GS";
        default:
            return "";
    }
    return "";
}

static ostream& operator << (std::ostream& strm, const Instruction& instr)
{
    if( instr.opcode.arg_format == NONE )
        return strm << instr.opcode.ascii_str;
    if( instr.opcode.arg_format == MODRM )
    {
        std::string additInfo;
        if( (instr.opcode.op & (1 << 0)) == false )
            additInfo += " BYTE ";
        additInfo +=  transReg(instr.special) + " ";
        
        return strm << instr.opcode.ascii_str << " " << instr.dest_str << ", " << instr.source_str << " (" << additInfo << ")";
    }
    if( instr.opcode.arg_format == IMM )
    {
        return strm << instr.opcode.ascii_str << ", " << instr.argument;
    }
    if( instr.opcode.arg_format == JMP_REL )
    {
        return strm << instr.opcode.ascii_str << " " << "0x" << std::hex << instr.argument;
    }
    if( instr.opcode.arg_format == MODRM_IMM )
    {
        return strm << instr.opcode.ascii_str << " " << instr.dest_str << ", " << "0x" << std::hex << instr.argument;
    }
    throw std::runtime_error("Unq");
}

struct opcode_modrm_helper_t {
    uint8_t reg_v,mod_v,rmm_v,dir_v,siz_v;      // Struct for modrm byte.
};

template<typename T>
std::string bin(const T& t){
    std::string _tmp("0b");
    for(std::size_t i = 0;i < sizeof(t) * std::numeric_limits<T>::digits; ++i){
        if( t & (1 << i) ) _tmp += '1';
        else _tmp += '0';
    }
    return _tmp;
}

/** Very limited **/
static std::map<uint8_t, Opcode> opcodeMap
{
    /** JUMP **/
    {0xEB, Opcode(0xEB, 2, 2, "JMP", JMP_REL, 1)},  // One byte relative address.
    {0xE9, Opcode(0xE9, 3, 2, "JMP", JMP_REL, 2)},  // Two byte relative address.
    /** COMPARE **/
    {0x3C, Opcode(0x3C, 2, 2, "CMP AL", IMM, 1)},
    {0x3D, Opcode(0x3D, 3, 2, "CMP AX", IMM, 2)},
    /** UTIL **/
    {0x90, Opcode(0x90, 1, 2, "NOP", NONE, 0) },
    /** ADD **/
    {0x00, Opcode(0x00, 2, 2, "ADD", MODRM, 0) },
    {0x01, Opcode(0x01, 2, 2, "ADD", MODRM, 0) },
    {0x02, Opcode(0x02, 2, 2, "ADD", MODRM, 0) },
    {0x03, Opcode(0x03, 2, 2, "ADD", MODRM, 0) },
    {0x04, Opcode(0x04, 2, 2, "ADD AL", IMM, 1) },
    {0x05, Opcode(0x05, 3, 2, "ADD AX", IMM, 2) },
    /** PUSH **/
    {0x06, Opcode(0x06, 1, 2, "PUSH ES", NONE, 0) },
    {0x07, Opcode(0x07, 1, 2, "POP ES", NONE, 0) },
    {0x0E, Opcode(0x0E, 1, 2, "PUSH CS", NONE, 0)},
    /** OR **/
    {0x08, Opcode(0x08, 2, 2, "OR", MODRM, 0)},
    {0x09, Opcode(0x09, 2, 2, "OR", MODRM, 0)},
    {0x0A, Opcode(0x0A, 2, 2, "OR", MODRM, 0)},
    {0x0B, Opcode(0x0B, 2, 2, "OR", MODRM, 0)},
    {0x0C, Opcode(0x0C, 2, 2, "OR AL", IMM, 1)},
    {0x0D, Opcode(0x0D, 3, 2, "OR AX", IMM, 2)},
    /** IMM MOVES 8-bit **/
    {0xB0, Opcode(0xB0, 2, 2, "MOV AL", IMM, 1)},
    {0xB1, Opcode(0xB1, 2, 2, "MOV CL", IMM, 1)},
    {0xB2, Opcode(0xB2, 2, 2, "MOV DL", IMM, 1)},
    {0xB3, Opcode(0xB3, 2, 2, "MOV BL", IMM, 1)},
    {0xB4, Opcode(0xB4, 2, 2, "MOV AH", IMM, 1)},
    {0xB5, Opcode(0xB5, 2, 2, "MOV CH", IMM, 1)},
    {0xB6, Opcode(0xB6, 2, 2, "MOV DH", IMM, 1)},
    {0xB7, Opcode(0xB7, 2, 2, "MOV BH", IMM, 1)},
    /** IMM MOVES 16-bit **/
    {0xB8, Opcode(0xB8, 3, 2, "MOV AX", IMM, 2)},
    {0xB9, Opcode(0xB9, 3, 2, "MOV CX", IMM, 2)},
    {0xBA, Opcode(0xBA, 3, 2, "MOV DX", IMM, 2)},
    {0xBB, Opcode(0xBB, 3, 2, "MOV BX", IMM, 2)},
    {0xBC, Opcode(0xBC, 3, 2, "MOV SP", IMM, 2)},
    {0xBD, Opcode(0xBD, 3, 2, "MOV BP", IMM, 2)},
    {0xBE, Opcode(0xBE, 3, 2, "MOV SI", IMM, 2)},
    {0xBF, Opcode(0xBF, 3, 2, "MOV DI", IMM, 2)},
    /** REG/MEM or MEM/REG MOVES **/
    {0x88, Opcode(0x88, 2, 2, "MOV", MODRM, 0)},
    {0x89, Opcode(0x89, 2, 2, "MOV", MODRM, 0)},
    {0x8A, Opcode(0x8A, 2, 2, "MOV", MODRM, 0)},
    {0x8B, Opcode(0x8B, 2, 2, "MOV", MODRM, 0)},
    /** MEM/IMM **/
    {0xC7, Opcode(0xC7, 4, 2, "MOV", MODRM_IMM, 2)},
    
    
};

static std::map<uint8_t, imm_val_t > immedMap
{
    {0x04, {AL, IMMVAL} },
    {0x05, {AX, IMMVAL} },
    {0x0C, {AL, IMMVAL} },
    {0x0D, {AX, IMMVAL} },
    
    {0x3C, {AL, IMMVAL} },
    {0x3D, {AX, IMMVAL} },
    
    /** IMM MOVES 8-bit **/
    {0xB0, {AL, IMMVAL} },
    {0xB1, {CL, IMMVAL} },
    {0xB2, {DL, IMMVAL} },
    {0xB3, {BL, IMMVAL} },
    {0xB4, {AH, IMMVAL} },
    {0xB5, {CH, IMMVAL} },
    {0xB6, {DH, IMMVAL} },
    {0xB7, {BH, IMMVAL} },
    /** IMM MOVES 16-bit **/
    {0xB8, {AX, IMMVAL} },
    {0xB9, {CX, IMMVAL} },
    {0xBA, {DX, IMMVAL} },
    {0xBB, {BX, IMMVAL} },
    {0xBC, {SP, IMMVAL} },
    {0xBD, {BP, IMMVAL} },
    {0xBE, {SI, IMMVAL} },
    {0xBF, {DI, IMMVAL} },
    
    
};

static std::map<uint8_t, override_type_t> prefixes
{
    {0x2e, CS},
    {0x36, SS},
    {0x3e, DS},
    {0x26, ES},
    {0x64, FS},
    {0x65, GS},
    {0x66, NOT_SUPPORTED}, // Segment override
    {0x67, NOT_SUPPORTED}, // Address override
    {0xf3, NOT_SUPPORTED}, // REP, REPE
    {0xf2, NOT_SUPPORTED}, // REPNE
    {0xf0, NOT_SUPPORTED}  // LOCK
};

static std::map<uint8_t, std::pair<reg_type_t, std::string> > bigRegs
{
    { 0b00000000, { AX, "AX"} },
    { 0b00001000, { CX, "CX"} },
    { 0b00010000, { DX, "DX"} },
    { 0b00011000, { BX, "BX"} },
    { 0b00100000, { SP, "SP"} },
    { 0b00101000, { BP, "BP"} },
    { 0b00110000, { SI, "SI"} },
    { 0b00111000, { DI, "DI"} }
};

static std::map<uint8_t, std::pair<reg_type_t, std::string> > smallRegs
{
    { 0b00000000, { AL, "AL"} },
    { 0b00001000, { CL, "CL"} },
    { 0b00010000, { DL, "DL"} },
    { 0b00011000, { BL, "BL"} },
    { 0b00100000, { AH, "AH"} },
    { 0b00101000, { CH, "CH"} },
    { 0b00110000, { DH, "DH"} },
    { 0b00111000, { BH, "BH"} }
};

static std::map<uint8_t, std::pair<reg_type_t, std::string> > IndirectAdressing
{
    { 0b000, { ADDR_BX_SI, "[BX + SI]"} },
    { 0b001, { ADDR_BX_DI, "[BX + DI]"} },
    { 0b010, { ADDR_BP_SI, "[BP + SI]"} },
    { 0b011, { ADDR_BP_DI, "[BP + DI]"} },
    { 0b100, { ADDR_SI, "[SI]"} },
    { 0b101, { ADDR_DI, "[DI]"} },
    { 0b110, { ADDR_SWORD, "[sword]"} },
    { 0b111, { ADDR_BX, "[BX]"} }
};

typedef vector<Instruction> Code;
typedef vector<uint8_t> Bytes;
typedef opcode_modrm_helper_t oprmmod_t;
typedef std::map<uint8_t, std::pair<reg_type_t, std::string> > RegMap;

class Disasm16
{
public:
    Disasm16(const string& fileName);
    Code getCode();

private:
    Code loadCode();
    Opcode getOpcode(uint8_t byte);
    Instruction decode(const Opcode& opcode, uint8_t possiblePrefix);
    
    Instruction decodeModInstr(const Opcode& opcode, uint8_t possiblePrefix);
    Instruction decodeImmInstr(const Opcode& opcode, uint8_t possiblePrefix);
    Instruction decodeJmpInstr(const Opcode& opcode, uint8_t possiblePrefix);
    Instruction decodeMemImmInstr(const Opcode& opcode, uint8_t possiblePrefix);
    
    bool checkPrefix(uint8_t argument);
    bool loadBytes(const string &fileName);
    oprmmod_t decodeModRM(const Opcode& op, uint8_t modrm);
    RegMap* selectRegMap(const oprmmod_t& oprmmod);
    
    size_t i = 0;
    Bytes bytes;
};

#endif
