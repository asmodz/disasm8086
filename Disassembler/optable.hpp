//=============================================
#include "structs.hpp"
//=============================================

static std::map<uint8_t, opdata_t> opmap
{
    {0x00, opdata_t(0x00, "ADD", VAR_EB, VAR_GB, GRP_NONE, MODRM_ENC)},
    {0x01, opdata_t(0x01, "ADD", VAR_EV, VAR_GV, GRP_NONE, MODRM_ENC)},
    {0x02, opdata_t(0x02, "ADD", VAR_GB, VAR_EB, GRP_NONE, MODRM_ENC)},
    {0x03, opdata_t(0x03, "ADD", VAR_GV, VAR_EV, GRP_NONE, MODRM_ENC)},
    {0x04, opdata_t(0x04, "ADD", REG_AL, IMM_IB, GRP_NONE, REG_IMM_ENC)},
    {0x05, opdata_t(0x05, "ADD", REG_AX, IMM_IV, GRP_NONE, REG_IMM_ENC)},
    {0x06, opdata_t(0x06, "PUSH ES", REG_ES, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x07, opdata_t(0x07, "POP ES", REG_ES, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x08, opdata_t(0x08, "OR", VAR_EB, VAR_GB, GRP_NONE, MODRM_ENC)},
    {0x09, opdata_t(0x09, "OR", VAR_EV, VAR_GV, GRP_NONE, MODRM_ENC)},
    {0x0A, opdata_t(0x0A, "OR", VAR_GB, VAR_EB, GRP_NONE, MODRM_ENC)},
    {0x0B, opdata_t(0x0B, "OR", VAR_GV, VAR_EV, GRP_NONE, MODRM_ENC)},
    {0x0C, opdata_t(0x0C, "OR", REG_AL, IMM_IB, GRP_NONE, REG_IMM_ENC)},
    {0x0D, opdata_t(0x0D, "OR", REG_AX, IMM_IV, GRP_NONE, REG_IMM_ENC)},
    {0x0E, opdata_t(0x0E, "PUSH", REG_CS, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x0F, opdata_t(0x0F, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0x10, opdata_t(0x10, "ADC", VAR_EB, VAR_GB, GRP_NONE, MODRM_ENC)},
    {0x11, opdata_t(0x11, "ADC", VAR_EV, VAR_GV, GRP_NONE, MODRM_ENC)},
    {0x12, opdata_t(0x12, "ADC", VAR_GB, VAR_EB, GRP_NONE, MODRM_ENC)},
    {0x13, opdata_t(0x13, "ADC", VAR_GV, VAR_EV, GRP_NONE, MODRM_ENC)},
    {0x14, opdata_t(0x14, "ADC", REG_AL, IMM_IB, GRP_NONE, REG_IMM_ENC)},
    {0x15, opdata_t(0x15, "ADC", REG_AX, IMM_IV, GRP_NONE, REG_IMM_ENC)},
    {0x16, opdata_t(0x16, "PUSH", REG_SS, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x17, opdata_t(0x17, "POP", REG_SS, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x18, opdata_t(0x18, "SBB", VAR_EB, VAR_GB, GRP_NONE, MODRM_ENC)},
    {0x19, opdata_t(0x19, "SBB", VAR_EV, VAR_GV, GRP_NONE, MODRM_ENC)},
    {0x1A, opdata_t(0x1A, "SBB", VAR_GB, VAR_EB, GRP_NONE, MODRM_ENC)},
    {0x1B, opdata_t(0x1B, "SBB", VAR_GV, VAR_EV, GRP_NONE, MODRM_ENC)},
    {0x1C, opdata_t(0x1C, "SBB", REG_AL, IMM_IB, GRP_NONE, REG_IMM_ENC)},
    {0x1D, opdata_t(0x1D, "SBB", REG_AX, IMM_IV, GRP_NONE, REG_IMM_ENC)},
    {0x1E, opdata_t(0x1E, "PUSH", REG_DS, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x1F, opdata_t(0x1F, "POP", REG_DS, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x20, opdata_t(0x20, "AND", VAR_EB, VAR_GB, GRP_NONE, MODRM_ENC)},
    {0x21, opdata_t(0x21, "AND", VAR_EV, VAR_GV, GRP_NONE, MODRM_ENC)},
    {0x22, opdata_t(0x22, "AND", VAR_GB, VAR_EB, GRP_NONE, MODRM_ENC)},
    {0x23, opdata_t(0x23, "AND", VAR_GV, VAR_EV, GRP_NONE, MODRM_ENC)},
    {0x24, opdata_t(0x24, "AND", REG_AL, IMM_IB, GRP_NONE, REG_IMM_ENC)},
    {0x25, opdata_t(0x25, "AND", REG_AX, IMM_IV, GRP_NONE, REG_IMM_ENC)},
    {0x26, opdata_t(0x26, "ES:", PRX_ES, NONE, GRP_NONE, PRX_ENC)},
    {0x27, opdata_t(0x27, "DAA", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x28, opdata_t(0x28, "SUB", VAR_EB, VAR_GB, GRP_NONE, MODRM_ENC)},
    {0x29, opdata_t(0x29, "SUB", VAR_EV, VAR_GV, GRP_NONE, MODRM_ENC)},
    {0x2A, opdata_t(0x2A, "SUB", VAR_GB, VAR_EB, GRP_NONE, MODRM_ENC)},
    {0x2B, opdata_t(0x2B, "SUB", VAR_GV, VAR_EV, GRP_NONE, MODRM_ENC)},
    {0x2C, opdata_t(0x2C, "SUB", REG_AL, IMM_IB, GRP_NONE, REG_IMM_ENC)},
    {0x2D, opdata_t(0x2D, "SUB", REG_AX, IMM_IV, GRP_NONE, REG_IMM_ENC)},
    {0x2E, opdata_t(0x2E, "CS:", PRX_CS, NONE, GRP_NONE, PRX_ENC)},
    {0x2F, opdata_t(0x2F, "DAS", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x30, opdata_t(0x30, "XOR", VAR_EB, VAR_GB, GRP_NONE, MODRM_ENC)},
    {0x31, opdata_t(0x31, "XOR", VAR_EV, VAR_GV, GRP_NONE, MODRM_ENC)},
    {0x32, opdata_t(0x32, "XOR", VAR_GB, VAR_EB, GRP_NONE, MODRM_ENC)},
    {0x33, opdata_t(0x33, "XOR", VAR_GV, VAR_EV, GRP_NONE, MODRM_ENC)},
    {0x34, opdata_t(0x34, "XOR", REG_AL, IMM_IB, GRP_NONE, REG_IMM_ENC)},
    {0x35, opdata_t(0x35, "XOR", REG_AX, IMM_IV, GRP_NONE, REG_IMM_ENC)},
    {0x36, opdata_t(0x36, "SS:", PRX_SS, NONE, GRP_NONE, PRX_ENC)},
    {0x37, opdata_t(0x37, "AAA", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x38, opdata_t(0x38, "CMP", VAR_EB, VAR_GB, GRP_NONE, MODRM_ENC)},
    {0x39, opdata_t(0x39, "CMP", VAR_EV, VAR_GV, GRP_NONE, MODRM_ENC)},
    {0x3A, opdata_t(0x3A, "CMP", VAR_GB, VAR_EB, GRP_NONE, MODRM_ENC)},
    {0x3B, opdata_t(0x3B, "CMP", VAR_GV, VAR_EV, GRP_NONE, MODRM_ENC)},
    {0x3C, opdata_t(0x3C, "CMP", REG_AL, IMM_IB, GRP_NONE, REG_IMM_ENC)},
    {0x3D, opdata_t(0x3D, "CMP", REG_AX, IMM_IV, GRP_NONE, REG_IMM_ENC)},
    {0x3E, opdata_t(0x3E, "DS:", PRX_DS, NONE, GRP_NONE, PRX_ENC)},
    {0x3F, opdata_t(0x3F, "AAS", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x40, opdata_t(0x40, "INC AX", REG_AX, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x41, opdata_t(0x41, "INC CX", REG_CX, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x42, opdata_t(0x42, "INC DX", REG_DX, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x43, opdata_t(0x43, "INC BX", REG_BX, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x44, opdata_t(0x44, "INC SP", REG_SP, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x45, opdata_t(0x45, "INC BP", REG_BP, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x46, opdata_t(0x46, "INC SI", REG_SI, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x47, opdata_t(0x47, "INC DI", REG_DI, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x48, opdata_t(0x48, "DEC AX", REG_AX, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x49, opdata_t(0x49, "DEC CX", REG_CX, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x4A, opdata_t(0x4A, "DEC DX", REG_DX, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x4B, opdata_t(0x4B, "DEC BX", REG_BX, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x4C, opdata_t(0x4C, "DEC SP", REG_SP, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x4D, opdata_t(0x4D, "DEC BP", REG_BP, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x4E, opdata_t(0x4E, "DEC SI", REG_SI, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x4F, opdata_t(0x4F, "DEC DI", REG_DI, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x50, opdata_t(0x50, "PUSH AX", REG_AX, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x51, opdata_t(0x51, "PUSH CX", REG_CX, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x52, opdata_t(0x52, "PUSH DX", REG_DX, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x53, opdata_t(0x53, "PUSH BX", REG_BX, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x54, opdata_t(0x54, "PUSH SP", REG_SP, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x55, opdata_t(0x55, "PUSH BP", REG_BP, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x56, opdata_t(0x56, "PUSH SI", REG_SI, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x57, opdata_t(0x57, "PUSH DI", REG_DI, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x58, opdata_t(0x58, "POP AX", REG_AX, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x59, opdata_t(0x59, "POP CX", REG_CX, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x5A, opdata_t(0x5A, "POP DX", REG_DX, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x5B, opdata_t(0x5B, "POP BX", REG_BX, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x5C, opdata_t(0x5C, "POP SP", REG_SP, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x5D, opdata_t(0x5D, "POP BP", REG_BP, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x5E, opdata_t(0x5E, "POP SI", REG_SI, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x5F, opdata_t(0x5F, "POP DI", REG_DI, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x60, opdata_t(0x60, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0x61, opdata_t(0x61, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0x62, opdata_t(0x62, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0x63, opdata_t(0x63, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0x64, opdata_t(0x64, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0x65, opdata_t(0x65, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0x66, opdata_t(0x66, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0x67, opdata_t(0x67, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0x68, opdata_t(0x68, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0x69, opdata_t(0x69, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0x6A, opdata_t(0x6A, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0x6B, opdata_t(0x6B, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0x6C, opdata_t(0x6C, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0x6D, opdata_t(0x6D, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0x6E, opdata_t(0x6E, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0x6F, opdata_t(0x6F, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0x70, opdata_t(0x70, "JO", IMM_IB, NONE, GRP_NONE, JMP_ENC)},
    {0x71, opdata_t(0x71, "JNO", IMM_IB, NONE, GRP_NONE, JMP_ENC)},
    {0x72, opdata_t(0x72, "JB", IMM_IB, NONE, GRP_NONE, JMP_ENC)},
    {0x73, opdata_t(0x73, "JNB", IMM_IB, NONE, GRP_NONE, JMP_ENC)},
    {0x74, opdata_t(0x74, "JZ", IMM_IB, NONE, GRP_NONE, JMP_ENC)},
    {0x75, opdata_t(0x75, "JNZ", IMM_IB, NONE, GRP_NONE, JMP_ENC)},
    {0x76, opdata_t(0x76, "JBE", IMM_IB, NONE, GRP_NONE, JMP_ENC)},
    {0x77, opdata_t(0x77, "JA", IMM_IB, NONE, GRP_NONE, JMP_ENC)},
    {0x78, opdata_t(0x78, "JS", IMM_IB, NONE, GRP_NONE, JMP_ENC)},
    {0x79, opdata_t(0x79, "JNS", IMM_IB, NONE, GRP_NONE, JMP_ENC)},
    {0x7A, opdata_t(0x7A, "JPE", IMM_IB, NONE, GRP_NONE, JMP_ENC)},
    {0x7B, opdata_t(0x7B, "JPO", IMM_IB, NONE, GRP_NONE, JMP_ENC)},
    {0x7C, opdata_t(0x7C, "JL", IMM_IB, NONE, GRP_NONE, JMP_ENC)},
    {0x7D, opdata_t(0x7D, "JGE", IMM_IB, NONE, GRP_NONE, JMP_ENC)},
    {0x7E, opdata_t(0x7E, "JLE", IMM_IB, NONE, GRP_NONE, JMP_ENC)},
    {0x7F, opdata_t(0x7F, "JG", IMM_IB, NONE, GRP_NONE, JMP_ENC)},
    {0x80, opdata_t(0x80, "GRP1", VAR_EB, IMM_IB, GRP1, GRP_ENC)},
    {0x81, opdata_t(0x81, "GRP1", VAR_EV, IMM_IV, GRP1, GRP_ENC)},
    {0x82, opdata_t(0x82, "GRP1", NONE, NONE, GRP1, GRP_ENC)},
    {0x83, opdata_t(0x83, "GRP1", VAR_EV, IMM_IB, GRP1, GRP_ENC)},
    {0x84, opdata_t(0x84, "TEST", VAR_GB, VAR_EB, GRP_NONE, MODRM_ENC)},
    {0x85, opdata_t(0x85, "TEST", VAR_GV, VAR_EV, GRP_NONE, MODRM_ENC)},
    {0x86, opdata_t(0x86, "XCHG", VAR_GB, VAR_EB, GRP_NONE, MODRM_ENC)},
    {0x87, opdata_t(0x87, "XCHG", VAR_GV, VAR_EV, GRP_NONE, MODRM_ENC)},
    {0x88, opdata_t(0x88, "MOV", VAR_EB, VAR_GB, GRP_NONE, MODRM_ENC)},
    {0x89, opdata_t(0x89, "MOV", VAR_EV, VAR_GV, GRP_NONE, MODRM_ENC)},
    {0x8A, opdata_t(0x8A, "MOV", VAR_GB, VAR_EB, GRP_NONE, MODRM_ENC)},
    {0x8B, opdata_t(0x8B, "MOV", VAR_GV, VAR_EV, GRP_NONE, MODRM_ENC)},
    {0x8C, opdata_t(0x8C, "MOV", VAR_EV, VAR_SV, GRP_NONE, MODRM_SEG_ENC)},				// scase MOV [bx], cs | MOV <MODRM>, <SEGREG>
    {0x8D, opdata_t(0x8D, "LEA", VAR_GV, VAR_EV, GRP_NONE, MODRM_MEM_ENC)},				// scase lea
    {0x8E, opdata_t(0x8E, "MOV", VAR_SV, VAR_EV, GRP_NONE, MODRM_SEG_ENC)},				// scase mov (modseg), (modreg)
    {0x8F, opdata_t(0x8F, "POP", VAR_EV, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x90, opdata_t(0x90, "NOP", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x91, opdata_t(0x91, "XCHG", REG_CX, REG_AX, GRP_NONE, REG_REG_ENC)},
    {0x92, opdata_t(0x92, "XCHG", REG_DX, REG_AX, GRP_NONE, REG_REG_ENC)},
    {0x93, opdata_t(0x93, "XCHG", REG_BX, REG_AX, GRP_NONE, REG_REG_ENC)},
    {0x94, opdata_t(0x94, "XCHG", REG_SP, REG_AX, GRP_NONE, REG_REG_ENC)},
    {0x95, opdata_t(0x95, "XCHG", REG_BP, REG_AX, GRP_NONE, REG_REG_ENC)},
    {0x96, opdata_t(0x96, "XCHG", REG_SI, REG_AX, GRP_NONE, REG_REG_ENC)},
    {0x97, opdata_t(0x97, "XCHG", REG_DI, REG_AX, GRP_NONE, REG_REG_ENC)},
    {0x98, opdata_t(0x98, "CBW", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x99, opdata_t(0x99, "CWD", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x9A, opdata_t(0x9A, "CALL", NONE, NONE, GRP_NONE, RAW_SEG_RAW_OFF_ENC)},			// scase call 0xfafa:0xfefe
    {0x9B, opdata_t(0x9B, "WAIT", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x9C, opdata_t(0x9C, "PUSHF", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x9D, opdata_t(0x9D, "POPF", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x9E, opdata_t(0x9E, "SAHF", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0x9F, opdata_t(0x9F, "LAHF", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0xA0, opdata_t(0xA0, "MOV", REG_AL, RAW_SEG, GRP_NONE, AXAL_SEG_OFF_ENC)},			// scase mov al, [ds:offst]
    {0xA1, opdata_t(0xA1, "MOV", REG_AX, RAW_SEG, GRP_NONE, AXAL_SEG_OFF_ENC)},			// scase mov ax, [ds:offst]
    {0xA2, opdata_t(0xA2, "MOV", RAW_SEG, REG_AL, GRP_NONE, AXAL_SEG_OFF_ENC)},			// scase mov .., ...
    {0xA3, opdata_t(0xA3, "MOV", RAW_SEG, REG_AX, GRP_NONE, AXAL_SEG_OFF_ENC)},
    {0xA4, opdata_t(0xA4, "MOVSB", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0xA5, opdata_t(0xA5, "MOVSW", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0xA6, opdata_t(0xA6, "CMPSB", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0xA7, opdata_t(0xA7, "CMPSW", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0xA8, opdata_t(0xA8, "TEST", REG_AL, IMM_IB, GRP_NONE, REG_IMM_ENC)},
    {0xA9, opdata_t(0xA9, "TEST", REG_AX, IMM_IV, GRP_NONE, REG_IMM_ENC)},
    {0xAA, opdata_t(0xAA, "STOSB", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0xAB, opdata_t(0xAB, "STOSW", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0xAC, opdata_t(0xAC, "LODSB", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0xAD, opdata_t(0xAD, "LODSW", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0xAE, opdata_t(0xAE, "SCASB", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0xAF, opdata_t(0xAF, "SCASW", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0xB0, opdata_t(0xB0, "MOV", REG_AL, IMM_IB, GRP_NONE, REG_IMM_ENC)},
    {0xB1, opdata_t(0xB1, "MOV", REG_CL, IMM_IB, GRP_NONE, REG_IMM_ENC)},
    {0xB2, opdata_t(0xB2, "MOV", REG_DL, IMM_IB, GRP_NONE, REG_IMM_ENC)},
    {0xB3, opdata_t(0xB3, "MOV", REG_BL, IMM_IB, GRP_NONE, REG_IMM_ENC)},
    {0xB4, opdata_t(0xB4, "MOV", REG_AH, IMM_IB, GRP_NONE, REG_IMM_ENC)},
    {0xB5, opdata_t(0xB5, "MOV", REG_CH, IMM_IB, GRP_NONE, REG_IMM_ENC)},
    {0xB6, opdata_t(0xB6, "MOV", REG_DH, IMM_IB, GRP_NONE, REG_IMM_ENC)},
    {0xB7, opdata_t(0xB7, "MOV", REG_BH, IMM_IB, GRP_NONE, REG_IMM_ENC)},
    {0xB8, opdata_t(0xB8, "MOV", REG_AX, IMM_IV, GRP_NONE, REG_IMM_ENC)},
    {0xB9, opdata_t(0xB9, "MOV", REG_CX, IMM_IV, GRP_NONE, REG_IMM_ENC)},
    {0xBA, opdata_t(0xBA, "MOV", REG_DX, IMM_IV, GRP_NONE, REG_IMM_ENC)},
    {0xBB, opdata_t(0xBB, "MOV", REG_BX, IMM_IV, GRP_NONE, REG_IMM_ENC)},
    {0xBC, opdata_t(0xBC, "MOV", REG_SP, IMM_IV, GRP_NONE, REG_IMM_ENC)},
    {0xBD, opdata_t(0xBD, "MOV", REG_BP, IMM_IV, GRP_NONE, REG_IMM_ENC)},
    {0xBE, opdata_t(0xBE, "MOV", REG_SI, IMM_IV, GRP_NONE, REG_IMM_ENC)},
    {0xBF, opdata_t(0xBF, "MOV", REG_DI, IMM_IV, GRP_NONE, REG_IMM_ENC)},
    {0xC0, opdata_t(0xC0, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0xC1, opdata_t(0xC1, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0xC2, opdata_t(0xC2, "RET", IMM_IV, NONE, GRP_NONE, IMM_ENC)},
    {0xC3, opdata_t(0xC3, "RET", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0xC4, opdata_t(0xC4, "LES", VAR_GV, VAR_EV, GRP_NONE, MODRM_MEM_ENC)},
    {0xC5, opdata_t(0xC5, "LDS", VAR_GV, VAR_EV, GRP_NONE, MODRM_MEM_ENC)},
    {0xC6, opdata_t(0xC6, "MOV", VAR_EB, IMM_IB, GRP_NONE, MODRM_IMM_ENC)},
    {0xC7, opdata_t(0xC7, "MOV", VAR_EV, IMM_IV, GRP_NONE, MODRM_IMM_ENC)},
    {0xC8, opdata_t(0xC8, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0xC9, opdata_t(0xC9, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0xCA, opdata_t(0xCA, "RETF", IMM_IV, NONE, GRP_NONE, IMM_ENC)},
    {0xCB, opdata_t(0xCB, "RETF", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0xCC, opdata_t(0xCC, "INT", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0xCD, opdata_t(0xCD, "INT", IMM_IB, NONE, GRP_NONE, IMM_ENC)},
    {0xCE, opdata_t(0xCE, "INTO", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0xCF, opdata_t(0xCF, "IRET", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0xD0, opdata_t(0xD0, "GRP2", VAR_EB, NUM_ONE, GRP2, GRP_ENC)},
    {0xD1, opdata_t(0xD1, "GRP2", VAR_EV, NUM_ONE, GRP2, GRP_ENC)},
    {0xD2, opdata_t(0xD2, "GRP2", VAR_EB, REG_CL, GRP2, GRP_ENC)},
    {0xD3, opdata_t(0xD3, "GRP2", VAR_EV, REG_CL, GRP2, GRP_ENC)},
    {0xD4, opdata_t(0xD4, "AAM", IMM_IB, NONE, GRP_NONE, IMM_ENC)},
    {0xD5, opdata_t(0xD5, "AAD", IMM_IB, NONE, GRP_NONE, IMM_ENC)},
    {0xD6, opdata_t(0xD6, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0xD7, opdata_t(0xD7, "XLAT", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},
    {0xD8, opdata_t(0xD8, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0xD9, opdata_t(0xD9, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0xDA, opdata_t(0xDA, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0xDB, opdata_t(0xDB, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0xDC, opdata_t(0xDC, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0xDD, opdata_t(0xDD, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0xDE, opdata_t(0xDE, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0xDF, opdata_t(0xDF, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},
    {0xE0, opdata_t(0xE0, "LOOPNZ", IMM_IB, NONE, GRP_NONE, JMP_ENC)},
    {0xE1, opdata_t(0xE1, "LOOPZ", IMM_IB, NONE, GRP_NONE, JMP_ENC)},
    {0xE2, opdata_t(0xE2, "LOOP", IMM_IB, NONE, GRP_NONE, JMP_ENC)},
    {0xE3, opdata_t(0xE3, "JCXZ", IMM_IB, NONE, GRP_NONE, JMP_ENC)},
    {0xE4, opdata_t(0xE4, "IN", REG_AL, IMM_IB, GRP_NONE, REG_IMM_ENC)},
    {0xE5, opdata_t(0xE5, "IN", REG_AX, IMM_IB, GRP_NONE, REG_IMM_ENC)},
    {0xE6, opdata_t(0xE6, "OUT", IMM_IB, REG_AL, GRP_NONE, IMM_ENC)},
    {0xE7, opdata_t(0xE7, "OUT", IMM_IB, REG_AX, GRP_NONE, IMM_ENC)},
    {0xE8, opdata_t(0xE8, "CALL", IMM_IV, NONE, GRP_NONE, JMP_ENC)},
    {0xE9, opdata_t(0xE9, "JMP", IMM_IV, NONE, GRP_NONE, JMP_ENC)},
    {0xEA, opdata_t(0xEA, "JMP", RAW_SEG, RAW_OFS, GRP_NONE, RAW_SEG_RAW_OFF_ENC)},		// scase jmp 0xfefe:0xfafa
    {0xEB, opdata_t(0xEB, "JMP", IMM_IB, NONE, GRP_NONE, JMP_ENC)},					// ok
    {0xEC, opdata_t(0xEC, "IN", REG_AL, REG_DX, GRP_NONE, ONE_BYTE_ENC)},			// ok
    {0xED, opdata_t(0xED, "IN", REG_AX, REG_DX, GRP_NONE, ONE_BYTE_ENC)},			// ok
    {0xEE, opdata_t(0xEE, "OUT", REG_DX, REG_AL, GRP_NONE, ONE_BYTE_ENC)},			// ok
    {0xEF, opdata_t(0xEF, "OUT", REG_DX, REG_AX, GRP_NONE, ONE_BYTE_ENC)},			// ok
    {0xF0, opdata_t(0xF0, "LOCK", PRX_LOCK, NONE, GRP_NONE, PRX_ENC)},				// ok
    {0xF1, opdata_t(0xF1, "INVALID", INVALID, INVALID, GRP_NONE, INVALID_ENC)},		// -- 
    {0xF2, opdata_t(0xF2, "REPNZ", PRX_REPNZ, NONE, GRP_NONE, PRX_ENC)},			// ok
    {0xF3, opdata_t(0xF3, "REPZ", PRX_REPZ, NONE, GRP_NONE, PRX_ENC)},				// ok
    {0xF4, opdata_t(0xF4, "HLT", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},				// ok
    {0xF5, opdata_t(0xF5, "CMC", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},				// ok
    {0xF6, opdata_t(0xF6, "GRP3a", VAR_EB, NONE, GRP3a, GRP_ENC)},					// ok
    {0xF7, opdata_t(0xF7, "GRP3b", VAR_EV, NONE, GRP3b, GRP_ENC)},					// ok
    {0xF8, opdata_t(0xF8, "CLC", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},				// ok
    {0xF9, opdata_t(0xF9, "STC", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},				// ok
    {0xFA, opdata_t(0xFA, "CLI", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},				// ok
    {0xFB, opdata_t(0xFB, "STI", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},				// ok
    {0xFC, opdata_t(0xFC, "CLD", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},				// ok
    {0xFD, opdata_t(0xFD, "STD", NONE, NONE, GRP_NONE, ONE_BYTE_ENC)},				// ok
    {0xFE, opdata_t(0xFE, "GRP4", VAR_EB, NONE, GRP4, GRP_ENC)},						// ok
    {0xFF, opdata_t(0xFF, "GRP5", VAR_EV, NONE, GRP5, GRP_ENC)},						// ok

};


static std::map<uint8_t, std::map<uint8_t, grp_entry_t> > Groups
{
	{ GRP1, 
		{
			{ 0b000, grp_entry_t("ADD", NONE, NONE, MODRM_IMM_ENC) },
			{ 0b001, grp_entry_t("OR", NONE, NONE, MODRM_IMM_ENC) },
			{ 0b010, grp_entry_t("ADC", NONE, NONE, MODRM_IMM_ENC) },
			{ 0b011, grp_entry_t("SBB", NONE, NONE, MODRM_IMM_ENC) },
			{ 0b100, grp_entry_t("AND", NONE, NONE, MODRM_IMM_ENC) },
			{ 0b101, grp_entry_t("SUB", NONE, NONE, MODRM_IMM_ENC) },
			{ 0b110, grp_entry_t("XOR", NONE, NONE, MODRM_IMM_ENC) },
			{ 0b111, grp_entry_t("CMP", NONE, NONE, MODRM_IMM_ENC) },
			
		},
	},
	
	{ GRP2, 
		{
			{ 0b000, grp_entry_t("ROL", NONE, NONE, MODRM_ARG_ONE) },
			{ 0b001, grp_entry_t("ROR", NONE, NONE, MODRM_ARG_ONE) },
			{ 0b010, grp_entry_t("RCL", NONE, NONE, MODRM_ARG_ONE) },
			{ 0b011, grp_entry_t("RCR", NONE, NONE, MODRM_ARG_ONE) },
			{ 0b100, grp_entry_t("SHL", NONE, NONE, MODRM_ARG_ONE) },
			{ 0b101, grp_entry_t("SHR", NONE, NONE, MODRM_ARG_ONE) },
			{ 0b110, grp_entry_t("", INVALID, INVALID, INVALID_ENC) },
			{ 0b111, grp_entry_t("SAR", NONE, NONE, MODRM_ARG_ONE) },
		} 
	},
	
	{ GRP3a,
		 {
			{ 0b000, grp_entry_t("TEST", VAR_EB, IMM_IB, MODRM_IMM_ENC) },
			{ 0b001, grp_entry_t("", INVALID, INVALID, INVALID_ENC) },
			{ 0b010, grp_entry_t("NOT", NONE, NONE, MODRM_ONE_ARG) },
			{ 0b011, grp_entry_t("NEG", NONE, NONE, MODRM_ONE_ARG) },
			{ 0b100, grp_entry_t("MUL", NONE, NONE, MODRM_ONE_ARG) },
			{ 0b101, grp_entry_t("IMUL", NONE, NONE, MODRM_ONE_ARG) },
			{ 0b110, grp_entry_t("DIV", NONE, NONE, MODRM_ONE_ARG) },
			{ 0b111, grp_entry_t("IDIV", NONE, NONE, MODRM_ONE_ARG) },
		 } 
	},
	
	{ GRP3b, 
		{
			{ 0b000, grp_entry_t("TEST", VAR_EV, IMM_IB, MODRM_IMM_ENC) },
			{ 0b001, grp_entry_t("", INVALID, INVALID, INVALID_ENC) },
			{ 0b010, grp_entry_t("NOT", NONE, NONE, MODRM_ONE_ARG) },
			{ 0b011, grp_entry_t("NEG", NONE, NONE, MODRM_ONE_ARG) },
			{ 0b100, grp_entry_t("MUL", NONE, NONE, MODRM_ONE_ARG) },
			{ 0b101, grp_entry_t("IMUL", NONE, NONE, MODRM_ONE_ARG) },
			{ 0b110, grp_entry_t("DIV", NONE, NONE, MODRM_ONE_ARG) },
			{ 0b111, grp_entry_t("IDIV", NONE, NONE, MODRM_ONE_ARG) },
		} 
	},
	
	{ GRP4, 
		{
			{ 0b000, grp_entry_t("INC", NONE, NONE, MODRM_ONE_ARG) },		  // inc byte [bx]
			{ 0b001, grp_entry_t("DEC", NONE, NONE, MODRM_ONE_ARG) },		  // dec .........
			{ 0b010, grp_entry_t("", INVALID, INVALID, INVALID_ENC) },
			{ 0b011, grp_entry_t("", INVALID, INVALID, INVALID_ENC) },
			{ 0b100, grp_entry_t("", INVALID, INVALID, INVALID_ENC) },
			{ 0b101, grp_entry_t("", INVALID, INVALID, INVALID_ENC) },
			{ 0b110, grp_entry_t("", INVALID, INVALID, INVALID_ENC) },
			{ 0b111, grp_entry_t("", INVALID, INVALID, INVALID_ENC) },
		} 
	},
	
	{ GRP5, 
		{
			{ 0b000, grp_entry_t("INC", NONE, NONE, MODRM_ONE_ARG) },			// inc word [bx]
			{ 0b001, grp_entry_t("DEC", NONE, NONE, MODRM_ONE_ARG) },			// dec .........
			{ 0b010, grp_entry_t("CALL", NONE, NONE, MODRM_ONE_ARG) },		// call [0xFaFe]
			{ 0b011, grp_entry_t("CALL", IMM_IB, NONE, INVALID_ENC) },				// scase call [0xfafa] -> \xff \x16 \xfa \xfa
			{ 0b100, grp_entry_t("JMP", NONE, NONE, MODRM_ONE_ARG) },
			{ 0b101, grp_entry_t("JMP", IMM_MEM, NONE, INVALID_ENC) },		// scase jmp -||-
			{ 0b110, grp_entry_t("PUSH", NONE, NONE, MODRM_ONE_ARG) },
			{ 0b111, grp_entry_t("", INVALID, INVALID, INVALID_ENC) },
		} 
	},
	
};

static std::map<uint8_t, std::map<uint8_t, val_type_t> > ModRmMap
{
	{ 
		NO_DISPLACEMENT,
		{
			{ 0b000, ADDR_BX_SI },
			{ 0b001, ADDR_BX_DI },
			{ 0b010, ADDR_BP_SI },
			{ 0b011, ADDR_BP_DI },
			{ 0b100, ADDR_SI },
			{ 0b101, ADDR_DI },
			{ 0b110, ADDR_SWORD },
			{ 0b111, ADDR_BX },
		}
	},
	
	{
		BYTE_DISPLACEMENT,
		{
			{ 0b000, ADDR_BX_SI_SBYTE },
			{ 0b001, ADDR_BX_DI_SBYTE },
			{ 0b010, ADDR_BP_SI_SBYTE },
			{ 0b011, ADDR_BP_DI_SBYTE },
			{ 0b100, ADDR_SI_SBYTE },
			{ 0b101, ADDR_DI_SBYTE },
			{ 0b110, ADDR_BP_SBYTE },
			{ 0b111, ADDR_BX_SBYTE },
		}
	},
	
	{
		WORD_DISPLACEMENT,
		{
			{ 0b000, ADDR_BX_SI_SWORD },
			{ 0b001, ADDR_BX_DI_SWORD },
			{ 0b010, ADDR_BP_SI_SWORD },
			{ 0b011, ADDR_BP_DI_SWORD },
			{ 0b100, ADDR_SI_SWORD },
			{ 0b101, ADDR_DI_SWORD },
			{ 0b110, ADDR_BP_SWORD },
			{ 0b111, ADDR_BX_SWORD },
		}
	},

};

typedef std::map<uint8_t, val_type_t> RegisterMap;

static std::map<uint8_t, val_type_t>  Reg8Map
{
	{ 0b000, REG_AL },
	{ 0b001, REG_CL },
	{ 0b010, REG_DL },
	{ 0b011, REG_BL },
	{ 0b100, REG_AH },
	{ 0b101, REG_CH },
	{ 0b110, REG_DH },
	{ 0b111, REG_BH },	
};

static std::map<uint8_t, val_type_t>  Reg16Map
{
	{ 0b000, REG_AX },
	{ 0b001, REG_CX },
	{ 0b010, REG_DX },
	{ 0b011, REG_BX },
	{ 0b100, REG_SP },
	{ 0b101, REG_BP },
	{ 0b110, REG_SI },
	{ 0b111, REG_DI },
};

static std::map<uint8_t, val_type_t> segmap
{
	{ 0b000, REG_ES },
	{ 0b001, REG_CS },
	{ 0b010, REG_SS },
	{ 0b011, REG_DS },
};


static prefix_type_t prefixType(uint8_t byte)
{
	opdata_t opd = opmap[byte];
	if(opd.dst == PRX_CS || opd.dst == PRX_DS || opd.dst == PRX_ES || opd.dst == PRX_SS)
		return FIX_REG;
	if(opd.dst == PRX_REPNZ || opd.dst == PRX_REPZ)
		return FIX_REP;
	if(opd.dst == PRX_LOCK)
		return FIX_LOCK;
	
	return FIX_NONE;
}
