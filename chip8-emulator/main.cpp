#include "cpu.h"
#include "SDL\include\SDL.h"
#undef _main

int main(int argc, char* argv[])
{
	CPU cpu;
	FILE* rom = fopen(argv[1], "rb");

	Initialization(&cpu, rom);
	InstructionCycle(&cpu);
	return 0;
}
