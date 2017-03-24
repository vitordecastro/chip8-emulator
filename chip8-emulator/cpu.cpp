#include "cpu.h"

void Initialization(CPU* cpu, FILE* rom)
{
	cpu->PC = START_RAM;
	cpu->I = 0;
	cpu->SP = 0;
	cpu->Opcode = 0;

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

//Increment PC: It increments +2 because each instruction is 2 bytes long 
void IncrementPC(CPU *cpu)
{
	cpu->PC += 2;
}

void InstructionCycle(CPU* cpu)
{
	//Fetch Code
	cpu->Opcode = cpu->Memory[cpu->PC] << 8 | cpu->Memory[cpu->PC + 1];

	// Decode opcode

	const unsigned short operation = cpu->Opcode & 0xF000;

	switch (operation)
	{
		case 0x0000:
			switch (cpu->Opcode & 0x000F)
			{
				case 0x0000: // 00E0: Clears the screen        
					break;
				case 0x000E: // 00EE: Returns from subroutine          
					break;
				default:
					printf("Unknown opcode [0x0000]: 0x%X\n", cpu->Opcode);
			}
			break;
		case 0x1000: // 1NNN:	Jumps to address NNN.
			cpu->PC = GET_NNN(cpu->Opcode);
			break;
		case 0x2000: // 2NNN:	Calls subroutine at NNN.
			cpu->Stack[cpu->SP] = cpu->PC;
			cpu->SP++;
			cpu->PC = GET_NNN(cpu->Opcode);
			break;
		case 0x3000: // 3XNN:	Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to skip a code block)
			if(cpu->V[GET_X(cpu->Opcode)] == GET_NN(cpu->Opcode))
			{
				IncrementPC(cpu);
			}
			break;
		case 0x4000: // 4XNN:	Skips the next instruction if VX doesn't equal NN. (Usually the next instruction is a jump to skip a code block)
			if (cpu->V[GET_X(cpu->Opcode)] != GET_NN(cpu->Opcode))
			{
				IncrementPC(cpu);
			}
			break;
		case 0x5000: // 5XY0:	Skips the next instruction if VX equals VY. (Usually the next instruction is a jump to skip a code block)
			if (cpu->V[GET_X(cpu->Opcode)] == cpu->V[GET_Y(cpu->Opcode)])
			{
				IncrementPC(cpu);
			}
			break;
		case 0x6000: // 6XNN:	Sets VX to NN
			cpu->V[GET_X(cpu->Opcode)] = GET_NN(cpu->Opcode);
			break;
		case 0x7000: // 7XNN:	Adds NN to VX
			cpu->V[GET_X(cpu->Opcode)] += GET_NN(cpu->Opcode);
			break;
	default:
		printf("Unknown opcode: 0x%X\n", cpu->Opcode);
	}
}