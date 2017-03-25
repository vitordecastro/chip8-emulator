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
		case 0x8000:
			switch (cpu->Opcode & 0x000F)
			{
				case 0x0000: // 8XY0:	Sets VX to the value of VY
					cpu->V[GET_X(cpu->Opcode)] = cpu->V[GET_Y(cpu->Opcode)];
					break;
				case 0x0001: // 8XY1:	Sets VX to VX or VY. (Bitwise OR operation) VF is reset to 0
					cpu->V[GET_X(cpu->Opcode)] |= cpu->V[GET_Y(cpu->Opcode)];
					//cpu->V[0x0F] = 0;
					break;
				case 0x0002: // 8XY2:	Sets VX to VX and VY. (Bitwise AND operation) VF is reset to 0
					cpu->V[GET_X(cpu->Opcode)] &= cpu->V[GET_Y(cpu->Opcode)];
					//cpu->V[0x0F] = 0;
					break;
				case 0x0003: // 8XY3:	Sets VX to VX xor VY. VF is reset to 0
					cpu->V[GET_X(cpu->Opcode)] ^= cpu->V[GET_Y(cpu->Opcode)];
					//cpu->V[0x0F] = 0;
					break;
				case 0x0004: // 8XY4:	Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't
					cpu->V[0xF] = cpu->V[GET_Y(cpu->Opcode)] > (0xFF - cpu->V[GET_X(cpu->Opcode)]);
					cpu->V[GET_X(cpu->Opcode)] += cpu->V[GET_Y(cpu->Opcode)];
					break;
				case 0x0005: // 8XY5:	VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't
					cpu->V[0xF] = cpu->V[GET_X(cpu->Opcode)] > cpu->V[GET_Y(cpu->Opcode)];
					cpu->V[GET_X(cpu->Opcode)] -= cpu->V[GET_Y(cpu->Opcode)];
					break;
				case 0x0006: // 8XY6:	Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift
					cpu->V[0xF] = cpu->V[GET_X(cpu->Opcode)] & 0x0001;
					cpu->V[GET_X(cpu->Opcode)] >>= 1;
					break;
				case 0x0007: // 8XY7:	Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't
					cpu->V[0xF] = (cpu->V[GET_Y(cpu->Opcode)] > cpu->V[GET_X(cpu->Opcode)]);
					cpu->V[GET_X(cpu->Opcode)] = cpu->V[GET_Y(cpu->Opcode)] - cpu->V[GET_X(cpu->Opcode)];
					break;
				case 0x000E: // 8XYE:	Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift
					cpu->V[0xF] = cpu->V[GET_X(cpu->Opcode)] >> 7;
					cpu->V[GET_X(cpu->Opcode)] >>= 1;
					break;
				default:
					printf("Unknown opcode [0x0000]: 0x%X\n", cpu->Opcode);
			}
			break;
		default:
			printf("Unknown opcode: 0x%X\n", cpu->Opcode);
	}
}