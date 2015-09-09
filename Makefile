
CFLAGS=--std=c++0x  -Winvalid-pch -Wno-unused-function
CC=g++
ASM=nasm

main: link

	
all: Executable16 Disasm16
	$(CC) $(CFLAGS) -g main.cpp -o main obj/Executable16.o obj/Disasm16.o 

link:
	$(CC) $(CFLAGS) -g main.cpp -o main obj/Executable16.o obj/Disasm16.o 

Executable16: Executable16.hpp
	$(CC) $(CFLAGS) -c -g Disassembler/Executable16.cpp -o obj/Executable16.o
	
Disasm16: 
	$(CC) $(CFLAGS) -c Disassembler/Disasm16.cpp -o obj/Disasm16.o
	$(CC) $(CFLAGS)  main.cpp -o main obj/Disasm16.o 
	
test: 
	$(ASM) -f bin test-binary/test.asm -o test-binary/test.bin
	echo "objdump -M intel -D -b binary -m i8086 test-binary/test.bin" > redir

optable:
	g++ -x c++-header -gdwarf-2 -o obj/optable.hpp.gch -c Disassembler/optable.hpp --std=c++11 

pch: optable
