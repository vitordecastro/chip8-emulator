#include "cpu.h"
#include "SDL\include\SDL.h"
#undef _main

int main(int argc, char* argv[])
{
	CPU cpu;
	FILE rom;

	Initialization(&cpu, &rom);
	InstructionCycle(&cpu);
	return 0;
}
