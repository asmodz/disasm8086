#pragma once
#include "optable.hpp"


namespace Engine
{
	
typedef std::vector<uint8_t> Bytearray;
typedef std::vector<instruction_t> Instructions;

class Disasm8086
{
	public:
		Disasm8086(const std::string&);
			
	private:
		int reconInstructionSize(const opdata_t&) ;
		int reconDisplacementSize(const modrm_type_t&) ;
		void decodeAll();
		bool readBytes(const std::string&);
		inline bool allParsed() const;
		
		

		modrm_type_t decodeModRM(uint8_t) const;
		prefix_pack_t grabPrefixes();
		RegisterMap* const selectRegisterMapBySize(const opdata_t&);
				
		instruction_t decodeModRMInstr(opdata_t&)    throw(EncodeError);
		instruction_t decodeGroupInstr(opdata_t&)    throw(EncodeError);
		instruction_t decodeOneArgInstr(opdata_t&)   throw(EncodeError);
		instruction_t decodeModSegInstr(opdata_t&)   throw(EncodeError);
		instruction_t decodeModRMImmInstr(opdata_t&) throw(EncodeError);
		instruction_t decodeModRMemInstr(opdata_t&)  throw(EncodeError);
		
		
		instruction_t decodeImmInstr(opdata_t&);
		instruction_t decodeJmpInstr(opdata_t&);
		instruction_t decodeRegImmInstr(opdata_t&);
		instruction_t decodeRawJmpInstr(opdata_t&);
		instruction_t decodeAxAxSegInstr(opdata_t&);
		
		instruction_t dataByte();
		
		Bytearray bytes;
		unsigned int byteNo = 0;
		Instructions code;	
};
} // Engine

