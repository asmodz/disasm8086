#pragma once

#include <map>
#include <array>
#include <string>
#include <bitset>
#include <vector>
#include <fstream>
#include <iostream>
#include <exception>
#include <stdint.h>
#include <ctype.h>

#define GRP_NONE 0
#define GRP1     1
#define GRP2     2
#define GRP3a    0x3a
#define GRP3b    0x3b
#define GRP4     0x4
#define GRP5     0x5

#define VAL_NO_DISPLACEMENT   2
#define VAL_BYTE_DISPLACEMENT 3
#define VAL_WORD_DISPLACEMENT 4

#define SPECIAL_CASE_SWORD 0b110

#define NO_DISPLACEMENT   0b00
#define BYTE_DISPLACEMENT 0b01
#define WORD_DISPLACEMENT 0b10
#define REGISTER_TYPE     0b11

#define WIDTH_BIT     0
#define DIRECTION_BIT 1

#define TO_REGISTER   1
#define FROM_REGISTER 0

#define LOGERR(x) (("[" + std::string(__func__) + "]:" ) + x)
 
union instr_payload
{
    int8_t   sbyte;
    uint8_t  ubyte;
    int16_t  sword;
    uint16_t uword;
};

struct modrm_type_t
{
    std::bitset<2> sel;			/* Selector */
    std::bitset<3> mod;			/* Mod grp  */
    std::bitset<3> reg;			/* Reg grp  */
};

enum val_type_t
{
    VAR_EB, VAR_EV,					// membyte, memword
    VAR_GB, VAR_GV,					// regbyte, regword
    IMM_IB, IMM_IV,					// immbyte, immword
    VAR_SV, IMM_MEM,				// segment by mod
    
    PRX_ES,PRX_CS,					// prefix..
    PRX_DS,PRX_SS,					// prefix
    PRX_LOCK,PRX_REPNZ,PRX_REPZ,	// prefix..
    
    
    REG_AL,REG_CL,
    REG_DL,REG_BL,
    REG_AH,REG_CH,
    REG_DH,REG_BH,
  
    REG_AX,REG_CX,
    REG_DX,REG_BX,
    REG_SP,REG_BP,
    REG_SI,REG_DI,
    
    REG_CS,REG_DS,
    REG_SS,REG_ES,					// regs ....
    
    				
    RAW_SEG,RAW_OFS,
    NUM_ONE,

    ADDR_BX_SI, ADDR_BX_DI,
    ADDR_BP_SI, ADDR_BP_DI,
    ADDR_SI, ADDR_DI, 		
    ADDR_BX, ADDR_SWORD,   

    ADDR_BX_SI_SBYTE, ADDR_BX_DI_SBYTE,
    ADDR_BP_SI_SBYTE, ADDR_BP_DI_SBYTE,
    ADDR_SI_SBYTE, ADDR_DI_SBYTE, 
    ADDR_BP_SBYTE, ADDR_BX_SBYTE, 

    ADDR_BX_SI_SWORD, ADDR_BX_DI_SWORD,
    ADDR_BP_SI_SWORD, ADDR_BP_DI_SWORD,
    ADDR_SI_SWORD, ADDR_DI_SWORD,
    ADDR_BP_SWORD, ADDR_BX_SWORD,
    NONE, INVALID
};

struct opval_t
{
	opval_t()
	{
	}
	opval_t(val_type_t _val)
		:  val(_val)
	{
	}
	std::string name;
	val_type_t val;
};

enum encode_type_t
{
	/* If instruction is coded on modrm byte **/
	// <prefix> <op> <modrm> <displacement>
	// Out: LOCK MOV ES:[BX+DI+0xFAFA], BX
	// Out: ADD CS:[BX+DI+0xFAFA], CX
	MODRM_ENC,
	/* If instruction is coded by modrm group mask **/
	// <prefix> <op-grp> <modrm> <displacement>
	GRP_ENC,
	/* If is REG<-IMM instruction **/
	// <prefix> <op> <modrm> <displacement> <value>
	// MOV [BX], 34
	REG_IMM_ENC,
	/* If is immediate instruction (with one argument) eg. call 0xb004 **/
	// <prefix> <op> <value>
	// JMP 0xF
	// INT 13H
	IMM_ENC,
	/* One byte instruction eg. NOP, SCAS **/
	// <prefix> <opcode>
	ONE_BYTE_ENC,
	/* Jump, needed to calculate imm address **/
	// Same as IMM_ENC, but is need to calculate absolute address
	JMP_ENC,
    /* ModRM but only mem allowed eg. lea **, [bx] **/
    // <prefix> <opcode> <modrm>
    // only syntax /REGISTER16/, /MEMORY/ allowed
	MODRM_MEM_ENC,
	/* In reg field of modrm coded segment **/
	// Same as MODRM_ENC, but in modrm
	MODRM_SEG_ENC,
	/* Modrm immediate value **/
    MODRM_IMM_ENC,
    /* RegisterRegister eg. XCHG CX, BX **/
	REG_REG_ENC,
	/* Raw segment and raw offset in instruction eg. call 0xfefa:0xfafe **/
	RAW_SEG_RAW_OFF_ENC,
	/* raw offset in instruction bytes eg. mov ax, [ds:0xfafe] **/
	AXAL_SEG_OFF_ENC,
	/* bitop on modrm by one **/
	MODRM_ARG_ONE,
	/* ModRM with one argument eg NOT, NEG **/
	MODRM_ONE_ARG,
	/* Invalid encoding **/
	INVALID_ENC,
	/* Prefix before instruction type **/
	PRX_ENC
};
// if instructionEncoding in 
enum prefix_type_t { FIX_REP, FIX_REG, FIX_LOCK, FIX_NONE };


struct prefix_pack_t
{
	val_type_t reg_fix = NONE;					// If register override
	val_type_t rep_fix = NONE;					// If rep override
	val_type_t lck_fix = NONE;					// If lock override
};

struct opdata_t 
{
opdata_t()
{}
opdata_t(uint8_t _opcode, const std::string& _name, val_type_t _dst, val_type_t _src, encode_type_t _encode)
    : opcode(_opcode), name(_name), src(_src), dst(_dst), encode(_encode)
{}
opdata_t(uint8_t _opcode, const std::string& _name, val_type_t _dst, val_type_t _src, int _grp_index, encode_type_t _encode)
    : opcode(_opcode), name(_name), src(_src), dst(_dst), grp_index(_grp_index), encode(_encode)
{}
    uint8_t opcode;
    std::string name;
    val_type_t src, dst;
    int grp_index;
    encode_type_t encode;
    inline bool dir() const {
		return (opcode & (1 << DIRECTION_BIT));
	}
	inline bool size() const {
		return (opcode & (1 << WIDTH_BIT));
	}
	
};

struct instruction_t
{
instruction_t(){}
instruction_t(opdata_t _data, int dis = 0, instr_payload _mload = instr_payload(),
			  instr_payload _poff = instr_payload(), instr_payload _pseg = instr_payload() )
		: data(_data), displacement(dis), main_load(_mload), p_offset(_poff), p_segment(_pseg)
	{}
	prefix_pack_t fixpack;							// Prefix
	opdata_t data;
	int displacement, position;
	instr_payload main_load;					// If immediate
	instr_payload p_offset;						// If offset
	instr_payload p_segment;					// If segment in (RAW_SEG_RAW_OFF_ENC)
};

class EncodeError : public std::exception
{
	virtual const char* what()
	{
		return "";
	}
	
};

static std::map<val_type_t, std::string> val_strings
{
	{ VAR_EB, "VAR_EB" },
	{ VAR_EV, "VAR_EV" },
	{ VAR_GB, "VAR_GB" },
	{ VAR_GV, "VAR_GV" },
	{ VAR_SV, "VAR_SV" },
	{ IMM_IV, "IMM_IV" },
	{ IMM_IB, "IMM_IB" },
	{ PRX_ES, "ES:" },
	{ PRX_CS, "CS:" },
	{ PRX_DS, "DS:" },
	{ VAR_EV, "EV:" },
	{ PRX_SS, "SS:" },
	{ PRX_LOCK, "LOCK" },
	{ PRX_REPNZ, "REPNZ" },
	{ PRX_REPZ, "REPZ" },

	{ REG_AL, "AL" },
	{ REG_AX, "AX" },
	{ REG_CL, "CL" },
	{ REG_CX, "CX" },
	{ REG_DL, "DL" },
	{ REG_DX, "DX" },
	{ REG_BL, "BL" },
	{ REG_BX, "BX" },
	{ REG_AH, "AH" },
	{ REG_CH, "CH" },
	{ REG_BP, "BP" },
	{ REG_DH, "DH" },
	{ REG_SI, "SI" },
	{ REG_BH, "BH" },
	{ REG_DI, "DI" },
	{ REG_CS, "CS" },
	{ REG_DS, "DS" },
	{ REG_SS, "SS" },
	{ PRX_SS, "SS" },
	{ REG_ES, "ES" },	
	{ REG_SP, "SP" },
	
	{ ADDR_BX_SI, "[BX+SI]" },
	{ ADDR_BX_DI, "[BX+DI]" },
	{ ADDR_BP_SI, "[BP+SI]" },
	{ ADDR_BP_DI, "[BP+DI]" },
	{ ADDR_SI, "[SI]"},
	{ ADDR_DI, "[DI]"},		
    { ADDR_BX, "[BX]"},
    { ADDR_SWORD,  ""},

    {ADDR_BX_SI_SBYTE, "[BX+SI"},
    {ADDR_BX_DI_SBYTE, "[BX+DI"},
    {ADDR_BP_SI_SBYTE, "[BP+SI"},
    {ADDR_BP_DI_SBYTE, "[BP+DI"},
	{ADDR_SI_SBYTE,"[SI"},
	{ADDR_DI_SBYTE,"[DI"}, 
    {ADDR_BP_SBYTE,"[BP"},
    {ADDR_BX_SBYTE,"[BX"},

    {ADDR_BX_SI_SWORD,"[BX+SI"},
    {ADDR_BX_DI_SWORD,"[BX+DI"},
    {ADDR_BP_SI_SWORD,"[BP+SI"},
    {ADDR_BP_DI_SWORD,"[BP+DI"},
    {ADDR_SI_SWORD,"[SI"},
    {ADDR_DI_SWORD,"[DI"},
    {ADDR_BP_SWORD,"[BP"},
    {ADDR_BX_SWORD,"[BX"},
	{ IMM_MEM, "VAR_MEM" },
	{ RAW_SEG, "RAW_SEG" },
	{ RAW_OFS, "RAW_OFS" },
	{ NUM_ONE, "1" },

	{ NONE, "NONE" },	
	{ INVALID, "INVALID" },
};

static std::map<encode_type_t, std::string> enc_strings
{
	{MODRM_ENC, "MODRM_ENCODE"},
	{GRP_ENC, "GRP_ENCODE"},
	{REG_IMM_ENC, "REG_IMM_ENCODE"}, 
	{IMM_ENC, "IMM_ENCODE"}, 
	{ONE_BYTE_ENC, "ONE_BYTE_ENCODE"},
	{MODRM_SEG_ENC, "MODRM_SEG_ENCODE"},
	{JMP_ENC, "JMP_ENCODE"},
	{MODRM_MEM_ENC, "MODRM_MEM_ENCODE"},
	{MODRM_SEG_ENC, "MODRM_SEG_ENCODE"},
	{MODRM_IMM_ENC, "MODRM_IMM_ENCODE"},
	
	{REG_REG_ENC, "REG_REG_ENCODE"}, 
	{RAW_SEG_RAW_OFF_ENC, "RAW_SEG_RAW_OFF_ENCODE"}, 
	{AXAL_SEG_OFF_ENC, "AXAL_ENCODE"}, 
};

struct grp_entry_t
{
grp_entry_t()
{}
grp_entry_t(const std::string& _name, val_type_t _v0, val_type_t _v1, encode_type_t _enc)
	: name(_name), v0(_v0), v1(_v1), encode(_enc)
{}
	std::string name;
	val_type_t v0, v1;
	encode_type_t encode;
};


static inline bool isRegisterVal(val_type_t vt)
{
	return (vt >= REG_AL && vt <= REG_ES);
}
static inline bool isSmallReg(val_type_t vt)
{
	return (vt >= REG_AL && vt <= REG_BH);
}
static inline bool isBigReg(val_type_t vt)
{
	return (vt >= REG_AX && vt <= REG_DI);
}
static inline bool isSegmentReg(val_type_t vt)
{
	return (vt >= REG_CS && vt <= REG_DS);
}
static inline bool isImmediateByte(val_type_t vt)
{
	return vt == IMM_IB;
}
static inline bool isImmediateWord(val_type_t vt)
{
	return vt == IMM_IV;
}

static bool hasPrefix(const prefix_pack_t& fixpack, int what)
{
	#define ANY_PREFIX  1
	#define LOCK_OR_REP 2
	#define REG_PREFIX  3
	
	if(what == ANY_PREFIX)
		return (fixpack.reg_fix != NONE || fixpack.lck_fix != NONE || fixpack.rep_fix != NONE);
	if(what == LOCK_OR_REP)
		return (fixpack.lck_fix != NONE || fixpack.rep_fix != NONE);
	if(what == REG_PREFIX)
		return (fixpack.reg_fix != NONE);
}

static std::string prefixString(const prefix_pack_t& fixpack)
{
	std::string full;
	if(fixpack.lck_fix != NONE)
		full += val_strings.at(fixpack.lck_fix) + " ";
	if(fixpack.rep_fix != NONE)
		full += val_strings.at(fixpack.rep_fix) + " ";	
	return full;
}

static std::string signedHexString(const instruction_t& instr, int what)
{
	#define DISPLACEMENT   1
	#define DISPLACEMENT_WITHOUT_SIGNS 2
	#define REGISTER_SIZE  3
	#define IMMEDIATE_SIZE 4
	std::array<char, 16> buffer;
	if(what == DISPLACEMENT)
	{
		std::string min, plu;
		(min = "-", plu = "+");
		
		if(instr.displacement == VAL_BYTE_DISPLACEMENT)
		{
			int8_t x = instr.p_offset.sbyte;
			snprintf(buffer.data(), buffer.size(), "%s0x%x", 
				x<0?min.c_str():plu.c_str(), x<0?-(unsigned)x:x);
		}
		if(instr.displacement == VAL_WORD_DISPLACEMENT)
		{
			int16_t x = instr.p_offset.sword;
			snprintf(buffer.data(), buffer.size(), "%s0x%x", 
				x<0?min.c_str():plu.c_str(), x<0?-(unsigned)x:x);
		}
		return std::string(buffer.data());
	}
	if(what == DISPLACEMENT_WITHOUT_SIGNS)
	{
		if(instr.displacement == VAL_BYTE_DISPLACEMENT)
		{
			uint8_t x = instr.p_offset.ubyte;
			snprintf(buffer.data(), buffer.size(), "0x%x", x);
		}
		if(instr.displacement == VAL_WORD_DISPLACEMENT)
		{
			uint16_t x = instr.p_offset.uword;
			snprintf(buffer.data(), buffer.size(), "0x%x", x);
		}
		return std::string(buffer.data());		
	}
	if(what == REGISTER_SIZE)
	{
		if(isSmallReg(instr.data.dst))
		{
			int8_t x = instr.main_load.sbyte;
			sprintf(buffer.data(), "%s0x%x", x<0?"-":"", x<0?-(unsigned)x:x);
		}
		else
		{
			int16_t x = instr.main_load.sword;
			sprintf(buffer.data(), "%s0x%x", x<0?"-":"", x<0?-(unsigned)x:x);
		}
		return std::string(buffer.data());
	}
	if(what == IMMEDIATE_SIZE)
	{
		if(isImmediateByte(instr.data.src))
		{
			int8_t x = instr.main_load.sbyte;
			sprintf(buffer.data(), "%s0x%x", x<0?"-":"", x<0?-(unsigned)x:x);
		}
		if(isImmediateWord(instr.data.src))
		{
			int16_t x = instr.main_load.sword;
			sprintf(buffer.data(), "%s0x%x", x<0?"-":"", x<0?-(unsigned)x:x);
		}
		return std::string(buffer.data());
	}
	throw std::runtime_error(LOGERR("Unknown type"));
}

// =====================================================================
// <reg>, <reg>
// <mem>, <reg>
// <reg>, <mem>
static std::string getModRmInstrString(const instruction_t& instr)
{
	std::string fullString, regOverride, sizeString, displString;
	bool sword = false;
	
	if(hasPrefix(instr.fixpack, ANY_PREFIX))
		fullString += prefixString(instr.fixpack);
	if(hasPrefix(instr.fixpack, REG_PREFIX))
		regOverride += val_strings.at(instr.fixpack.reg_fix);

	fullString += instr.data.name + " ";
	
	if( isRegisterVal(instr.data.dst) && isRegisterVal(instr.data.src) )
		return fullString + val_strings.at(instr.data.dst) + ", " + val_strings.at(instr.data.src);

	if(instr.data.dst == ADDR_SWORD || instr.data.src == ADDR_SWORD)
		displString = "[" + signedHexString(instr, DISPLACEMENT_WITHOUT_SIGNS) + "]", sword = true;
			
	sizeString = (instr.data.size())?"WORD":"BYTE";
	
	if(instr.data.dir() == TO_REGISTER)
	{
		if(instr.displacement == VAL_NO_DISPLACEMENT)
			displString += val_strings.at(instr.data.src);
		if( sword == false && (instr.displacement == VAL_BYTE_DISPLACEMENT || instr.displacement == VAL_WORD_DISPLACEMENT ))
			displString += val_strings.at(instr.data.src) + signedHexString(instr, DISPLACEMENT) + "]";
		
		fullString += val_strings.at(instr.data.dst) + ", ";
		fullString += sizeString + " " + regOverride;
		fullString += displString;		
	}
	if(instr.data.dir() == FROM_REGISTER)
	{
		if(instr.displacement == VAL_NO_DISPLACEMENT)
			displString += val_strings.at(instr.data.dst);
		if( sword == false && (instr.displacement == VAL_BYTE_DISPLACEMENT || instr.displacement == VAL_WORD_DISPLACEMENT ))
			displString += val_strings.at(instr.data.dst) + signedHexString(instr, DISPLACEMENT) + "]";		
		
		fullString += sizeString + " " + regOverride;
		fullString += displString + ", ";
		fullString += val_strings.at(instr.data.src);
	}
	return fullString;

}
#include <inttypes.h>
// <reg>, <imm>
static std::string getRegImmString(const instruction_t& instr)
{
	std::string hexString;
	hexString = signedHexString(instr, REGISTER_SIZE);
	return instr.data.name + " " + val_strings.at(instr.data.dst) + ", " + hexString;
}
// <imm>
static std::string getImmString(const instruction_t& instr)
{
	std::array<char, 16> buffer;
	
	// Out case
	if(instr.data.dst == IMM_IB && ( instr.data.src == REG_AL || instr.data.src == REG_AX ))
	{
		snprintf(buffer.data(), 16, "0x%x", instr.main_load.sbyte);
		return instr.data.name + " " + buffer.data() + ", " + val_strings.at(instr.data.src);
	}
	
	if(instr.data.dst == IMM_IB)
	{
		snprintf(buffer.data(), 16, "0x%x", instr.main_load.sbyte);	
		return instr.data.name + " " + buffer.data();
	}
	
	if(instr.data.dst == IMM_IV)
	{
		snprintf(buffer.data(), 16, "0x%x", instr.main_load.sword);
		return instr.data.name + " " + buffer.data();
	}

	throw std::runtime_error(LOGERR("Unknown dst type"));
}

static std::string getImmModString(const instruction_t& instr)
{
	std::string fullString, regOverride, sizeString, displString;

	if(hasPrefix(instr.fixpack, ANY_PREFIX))
		fullString += prefixString(instr.fixpack);
	if(hasPrefix(instr.fixpack, REG_PREFIX))
		regOverride += val_strings.at(instr.fixpack.reg_fix);

	fullString += instr.data.name + " ";

	sizeString = (instr.data.size())?"WORD":"BYTE";

	if(isRegisterVal(instr.data.dst))
	{
		fullString += val_strings.at(instr.data.dst) + ", ";
		fullString += signedHexString(instr, IMMEDIATE_SIZE);
		/* Display char */
		return fullString;
	}
	
	fullString += sizeString + " ";		// WORD/BYTE
	fullString += regOverride;			// RegOvr
	
	if(instr.data.dst == ADDR_SWORD)	// If sword case
	{
		fullString += "[" + signedHexString(instr, DISPLACEMENT_WITHOUT_SIGNS) + "]";
		fullString += ", " + signedHexString(instr, IMMEDIATE_SIZE);
		return fullString;
	}
	
	fullString += val_strings.at(instr.data.dst) + "";
	fullString += signedHexString(instr, DISPLACEMENT) 
		+ ((instr.displacement != VAL_NO_DISPLACEMENT)?"]":"");
	fullString += ", " + signedHexString(instr, IMMEDIATE_SIZE);
	return fullString;
}

static std::string getRegSegString(const instruction_t& instr)
{
	std::string fullString;
	if( isRegisterVal(instr.data.dst) && isRegisterVal(instr.data.src) )
		return instr.data.name + " " + val_strings.at(instr.data.dst) + ", " + val_strings.at(instr.data.src);
	
	if(instr.data.dst == ADDR_SWORD)
	{
		fullString += instr.data.name + " " + "WORD [" + signedHexString(instr, DISPLACEMENT_WITHOUT_SIGNS) + "]";
		fullString += ", " + val_strings.at(instr.data.src);
		return fullString;
	}
	fullString += instr.data.name + " " + "WORD " + val_strings.at(instr.data.dst);
	if(instr.displacement == VAL_NO_DISPLACEMENT)
	{
		fullString += ", " + val_strings.at(instr.data.src);
		return fullString;
	}
	fullString += signedHexString(instr, DISPLACEMENT) + "]";
	fullString += ", " + val_strings.at(instr.data.src);
	return fullString;
}

static std::string getAxAlSegString(const instruction_t& instr)
{
	std::string fullString, regOverride, sizeString, displString;
	/* If prefix != REGFIX */
	if(hasPrefix(instr.fixpack, ANY_PREFIX))
		fullString += prefixString(instr.fixpack);
	if(hasPrefix(instr.fixpack, REG_PREFIX))
		regOverride += val_strings.at(instr.fixpack.reg_fix);
	fullString += instr.data.name;
	sizeString = (instr.data.size())?"WORD":"BYTE";
	
	if(isRegisterVal(instr.data.dst))
	{
		fullString += " " + val_strings.at(instr.data.dst) + ", " ;
		fullString += sizeString + " " + regOverride;
		fullString += "[" + signedHexString(instr, DISPLACEMENT_WITHOUT_SIGNS) + "]";
		return fullString;
	}
	
	fullString += " " + sizeString + " " + regOverride;
	fullString += "[" + signedHexString(instr, DISPLACEMENT_WITHOUT_SIGNS) + "], ";
	fullString += val_strings.at(instr.data.src);

	return fullString;
}

static std::string getOneArgString(const instruction_t& instr)
{
	std::string fullString, regOverride, sizeString, displString;
	/* If prefix != REGFIX */
	if(hasPrefix(instr.fixpack, ANY_PREFIX))
		fullString += prefixString(instr.fixpack);
	if(hasPrefix(instr.fixpack, REG_PREFIX))
		regOverride += val_strings.at(instr.fixpack.reg_fix);
	/* Add instr name */
	fullString += instr.data.name + " ";
	/* No displacement or reg/reg adressing */
	if(isSmallReg(instr.data.dst) == false)
		sizeString = (instr.data.size())?"WORD ":"BYTE ";

	
	fullString += sizeString;
	fullString += regOverride;
	if(instr.data.dst == ADDR_SWORD)
	{
		fullString += "[" + signedHexString(instr, DISPLACEMENT_WITHOUT_SIGNS) + "]";
		return fullString;
	}
	fullString += val_strings.at(instr.data.dst);
	fullString += signedHexString(instr, DISPLACEMENT);
	if(instr.displacement != VAL_NO_DISPLACEMENT)
		fullString += "]";
	return fullString;
}

static std::string getArgOneString(const instruction_t& instr)
{
	std::string fullString, regOverride, sizeString, displString;
	/* If prefix != REGFIX */
	if(hasPrefix(instr.fixpack, ANY_PREFIX))
		fullString += prefixString(instr.fixpack);
	if(hasPrefix(instr.fixpack, REG_PREFIX))
		regOverride += val_strings.at(instr.fixpack.reg_fix);
	/* Add instr name */
	fullString += instr.data.name + " ";
	/* No displacement or reg/reg adressing */
	if( isRegisterVal(instr.data.dst) )
	{
		fullString += val_strings.at(instr.data.dst) + ", ";
		fullString += val_strings.at(instr.data.src);
		return fullString;
	}
	sizeString = (instr.data.size())?"WORD":"BYTE";
	fullString += sizeString + " " + regOverride;
	if(instr.data.dst == ADDR_SWORD)
	{
		fullString += "[" + signedHexString(instr, DISPLACEMENT_WITHOUT_SIGNS) + "], ";
		fullString += val_strings.at(instr.data.src);
		return fullString;
	}
	fullString += val_strings.at(instr.data.dst);
	fullString += signedHexString(instr, DISPLACEMENT);
	
	if(instr.displacement != VAL_NO_DISPLACEMENT)
		fullString += "], ";
	else
		fullString += ", ";
	
	fullString += val_strings.at(instr.data.src);
	
	return fullString;
}

static std::string getModRMemPrint(const instruction_t& instr)
{
	std::string fullString, regOverride, displString;
	/* If prefix != REGFIX */
	if(hasPrefix(instr.fixpack, ANY_PREFIX))
		fullString += prefixString(instr.fixpack);
	if(hasPrefix(instr.fixpack, REG_PREFIX))
		regOverride += val_strings.at(instr.fixpack.reg_fix);
	/* Add instr name */
	fullString += instr.data.name + " ";
	/* No displacement or reg/reg adressing */	
	if(instr.data.src == ADDR_SWORD)
	{
		fullString += val_strings.at(instr.data.dst);
		fullString += ", " + regOverride;
		fullString += "[" + signedHexString(instr, DISPLACEMENT_WITHOUT_SIGNS) + "]";
		return fullString;
	}
	fullString += val_strings.at(instr.data.dst) + ", ";
	fullString += regOverride;
	fullString += val_strings.at(instr.data.src);
	fullString += signedHexString(instr, DISPLACEMENT);
	if(instr.displacement != VAL_NO_DISPLACEMENT)
		fullString += "]";
	
	return fullString;
}

static std::string getImmJmpString(const instruction_t& instr)
{
	std::array<char, 10> buffer;
	
	if(instr.data.dst == IMM_IB)
	{
		snprintf(buffer.data(), 10, "0x%x", instr.p_offset.ubyte);
		return instr.data.name + " " + buffer.data();
	}
	if(instr.data.dst == IMM_IV)
	{
		snprintf(buffer.data(), 10, "0x%x", instr.p_offset.uword);
		return instr.data.name + " " + buffer.data();
	}
	
	throw std::runtime_error(LOGERR("Unknown dst type"));
}

static std::string getRawJmpString(const instruction_t& instr)
{
	std::array<char, 16> buffer;
	snprintf(buffer.data(), 16, "0x%x:0x%x", instr.p_segment.uword, instr.p_offset.uword);
	return instr.data.name + " " + buffer.data();
}

static std::string getDataString(const instruction_t& instr)
{
	return instr.data.name;
}

// =====================================================================

static std::string getOneByteString(const instruction_t& instr)
{
	return instr.data.name;
}

static std::ostream& oneBytePrint(std::ostream& strm, const instruction_t& instr)
{
	return strm << getOneByteString(instr);
}

static inline std::ostream& modrmPrint(std::ostream& strm, const instruction_t& instr)
{
	return strm << getModRmInstrString(instr);
}

static std::ostream& regImmPrint (std::ostream& strm, const instruction_t& instr)
{
	return strm << getRegImmString(instr);
}

static std::ostream& immPrint(std::ostream& strm, const instruction_t& instr)
{
	return strm << getImmString(instr);
}

static std::ostream& immModPrint(std::ostream& strm, const instruction_t& instr)
{
	return strm << getImmModString(instr);
}

static std::ostream& regSegPrint(std::ostream& strm, const instruction_t& instr)
{
	return strm << getRegSegString(instr);
}

static std::ostream& immJmpPrint(std::ostream& strm, const instruction_t& instr)
{
	return strm << getImmJmpString(instr);
}

static std::ostream& axAlSegPrint(std::ostream& strm, const instruction_t& instr)
{
	return strm << getAxAlSegString(instr);
}

static std::ostream& regregPrint(std::ostream& strm, const instruction_t& instr)
{
	return strm << instr.data.name << " " << val_strings.at(instr.data.dst) << ", " << val_strings.at(instr.data.src);
}

static std::ostream& oneArgPrint(std::ostream& strm, const instruction_t& instr)
{
	return strm << getOneArgString(instr);
}

static std::ostream& argOnePrint(std::ostream& strm, const instruction_t& instr)
{
	return strm << getArgOneString(instr);
}

static std::ostream& printGrpInstr(std::ostream& strm, const instruction_t& instr)
{
	if(instr.data.encode == MODRM_IMM_ENC)
		return immModPrint(strm, instr);
	if(instr.data.encode == MODRM_ONE_ARG)
		return oneArgPrint(strm, instr);
	if(instr.data.encode == MODRM_ARG_ONE)
		return argOnePrint(strm, instr);
}

static std::ostream& modrmMemPrint(std::ostream& strm, const instruction_t& instr)
{
	return strm << getModRMemPrint(instr);
}

static std::ostream& rawSegRawOff(std::ostream& strm, const instruction_t& instr)
{
	return strm << getRawJmpString(instr);
}

static std::ostream& dataPrint(std::ostream& strm, const instruction_t& instr)
{
	return strm << getDataString(instr);
}

// =====================================================================

static std::ostream& operator << (std::ostream& strm, const instruction_t& instr)
{
	switch(instr.data.encode)
	{
		case GRP_ENC:
			return printGrpInstr(strm, instr);
			break;
		case ONE_BYTE_ENC:
			return oneBytePrint(strm, instr);
			break;
		case MODRM_ENC:
			return modrmPrint(strm, instr);
			break;
		case REG_IMM_ENC:
			return regImmPrint(strm, instr);
			break;
		case IMM_ENC:
			return immPrint(strm, instr);
			break;
		case MODRM_IMM_ENC:
			return immModPrint(strm, instr);
			break;
		case JMP_ENC:
			return immJmpPrint(strm, instr);
			break;
		case REG_REG_ENC:
			return regregPrint(strm, instr);
			break;
		case MODRM_ONE_ARG:
			return oneArgPrint(strm, instr);
			break;
		case MODRM_ARG_ONE:
			return argOnePrint(strm, instr);
			break;
		case MODRM_SEG_ENC:
			return regSegPrint(strm, instr);
			break;
		case AXAL_SEG_OFF_ENC:
			return axAlSegPrint(strm, instr);
			break;
		case MODRM_MEM_ENC:
			return modrmMemPrint(strm, instr);
			break;
		case RAW_SEG_RAW_OFF_ENC:
			return rawSegRawOff(strm, instr);
			break;
		case INVALID_ENC:
			return dataPrint(strm, instr);
			break;
	}
	return strm << "???";
}
