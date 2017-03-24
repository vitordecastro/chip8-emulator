#include "cpu.h"

void Initialization(CPU* cpu, FILE* rom)
{
	cpu->PC = START_RAM;
	cpu->I = 0;
	cpu->SP = 0;
	opcode = 0;

	LoadROM(cpu, rom);
	LoadFontSets(cpu);
}

void LoadFontSets(CPU* cpu)
{
	int i;
	int sizeSprites = (sizeof(sprites) / sizeof(unsigned char));
	for (i = 0; i < sizeSprites; i++)
	{
		cpu->Memory[i] = sprites[i];
	}
}

void LoadROM(CPU* cpu, FILE* rom)
{
	int byte = 0, i;
	for (i = 0; (byte = fgetc(rom)) != EOF; i++)
	{
		cpu->Memory[i + START_RAM] = byte;
	}
}