#include "Disassembler/Disasm16.hpp"
using namespace std;
int main()
{
	Engine::Disasm8086 disassembler("test-binary/test.bin");
	for(Instruction instr : disassembler)
	{
		std::cout << std::hex << instr.position 
		          << ":" << instr << std::endl;
	}
}
