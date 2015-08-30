#include "Executable16.hpp"
Exec16::Exec16(const string& fileName)
	: disassembler(fileName) 
{
	code = std::move(disassembler.getCode());
}

Code::iterator Exec16::begin()
{
	return code.begin();
}

Code::iterator Exec16::end()
{
	return code.end();
}
