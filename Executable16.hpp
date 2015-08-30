#ifndef EXEC16
#define EXEC16

#include "Disasm16.hpp"
#include <string>
using namespace std;

class Exec16
{
public:
	Exec16(const string& fileName);
	Code::iterator begin();
	Code::iterator end();
private:
	Disasm16 disassembler;
	Code code;
};

#endif
