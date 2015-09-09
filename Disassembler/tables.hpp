#pragma once
#include "structs.hpp"
#define NO_GROUP 0x00
#include <map>

// Opcode(hasModrm?, Opcode, HasGroup?)
static std::map<uint8_t, Opcode> opcodes
{
	{0x00, Opcode(true, 0x00, NO_GROUP)},	// add mem8,  reg8
	{0x01, Opcode(true, 0x01, NO_GROUP)},	// add mem16, reg16
};

static std::map<uint8_t, uint8_t> opcodeFullLength
{

	
};

static std::map<uint8_t, bool> opcodeGroup
{
	{0x80, true},		
	{0x81, true},
	{0x83, true},
};
//std::map<uint8_t, 
