
CFLAGS=--std=c++0x -Wall 
CC=g++
ASM=nasm

main: link
	
all: Executable16 Disasm16
	$(CC) $(CFLAGS) -g main.cpp -o main Executable16.o Disasm16.o

link:
	$(CC) $(CFLAGS) -g main.cpp -o main Executable16.o Disasm16.o

Executable16: Executable16.hpp
	$(CC) $(CFLAGS) -c -g Executable16.cpp
	
Disasm16: Disasm16.hpp
	$(CC) $(CFLAGS) -c -g Disasm16.cpp 

test: test.asm
	$(ASM) -f bin test.asm -o test.bin
	echo "objdump -M intel -D -b binary -m i8086 test.bin" > redir

