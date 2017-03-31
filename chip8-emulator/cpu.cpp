#include "cpu.h"

void Initialization(CPU* cpu, FILE* rom)
{
	cpu->PC = START_RAM;
	cpu->I = 0;
	cpu->SP = 0;
	cpu->Opcode = 0;

	for (int i = 0; i < W*H; i++)
		cpu->Display[i] = 0;

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

	IncrementPC(cpu);

	// Decode opcode

	unsigned short operation = cpu->Opcode & 0xF000;

	switch (operation)
	{
	case 0x0000:
		switch (cpu->Opcode & 0x000F)
		{
		case 0x0000: // 00E0: Clears the screen
			for (int position = 0; position < W*H; position++)
				cpu->Display[position] = 0;
			break;
		case 0x000E: // 00EE: Returns from subroutine          
			cpu->PC = cpu->Stack[cpu->SP--];
			break;
		default:
			printf("Unknown opcode [0x0000]: 0x%X\n", cpu->Opcode);
		}
		break;
	case 0x1000: // 1NNN:	Jumps to address NNN.
		cpu->PC = GET_NNN(cpu->Opcode);
		break;
	case 0x2000: // 2NNN:	Calls subroutine at NNN.
		cpu->Stack[++cpu->SP] = cpu->PC;
		cpu->PC = GET_NNN(cpu->Opcode);
		break;
	case 0x3000: // 3XNN:	Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to skip a code block)
		if (cpu->V[GET_X(cpu->Opcode)] == GET_NN(cpu->Opcode))
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
			cpu->V[GET_X(cpu->Opcode)] <<= 1;
			break;
		default:
			printf("Unknown opcode [0x0000]: 0x%X\n", cpu->Opcode);
		}
		break;
	case 0x9000: // 9XY0:	Skips the next instruction if VX doesn't equal VY. (Usually the next instruction is a jump to skip a code block)
		if (cpu->V[GET_X(cpu->Opcode)] != GET_Y(cpu->Opcode))
		{
			IncrementPC(cpu);
		}
		break;
	case 0xA000: // ANNN:	Sets I to the address NNN
		cpu->I = GET_NNN(cpu->Opcode);
		break;
	case 0xB000: // BNNN:	Jumps to the address NNN plus V0
		cpu->PC = GET_NNN(cpu->Opcode) + cpu->V[0x0];
		break;
	case 0xC000: // CNNN:	Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN
		cpu->V[GET_X(cpu->Opcode)] = rand() & GET_NN(cpu->Opcode);
		break;
	case 0XD000: // DXYN:	Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels
	{
		unsigned short coordinateX = cpu->V[GET_X(cpu->Opcode)];
		unsigned short coordinateY = cpu->V[GET_Y(cpu->Opcode)];
		unsigned short spriteHeight = GET_N(cpu->Opcode);
		unsigned short pixely;

		cpu->V[0xF] = 0;

		for (int yline = 0; yline < spriteHeight; yline++) //???????
		{
			pixely = cpu->Memory[cpu->I + yline];
			for (int xline = 0; xline < 8; xline++)
			{
				if ((pixely & (0x80 >> xline)) != 0)
				{
					if (cpu->Display[(coordinateX + xline + ((coordinateY + yline) * 64))] == 1)
					{
						cpu->V[0xF] = 1;
					}
					cpu->Display[coordinateX + xline + ((coordinateY + yline) * 64)] ^= 1;
				}
			}
		}
	}
	break;
	case 0xE000:
		switch (cpu->Opcode & 0x00FF)
		{
		case 0x009E: // EX9E:	Skips the next instruction if the key stored in VX is pressed. (Usually the next instruction is a jump to skip a code block)
			if (cpu->Keys[cpu->V[GET_X(cpu->Opcode)]] == 1)
				IncrementPC(cpu);
			break;
		case 0x00A1: // EXA1:	Skips the next instruction if the key stored in VX isn't pressed. (Usually the next instruction is a jump to skip a code block)
			if (cpu->Keys[cpu->V[GET_X(cpu->Opcode)]] == 0)
				IncrementPC(cpu);
			break;
		default:
			printf("Unknown opcode: 0x%X\n", cpu->Opcode);
		}
		break;
	case 0xF000:
		switch (cpu->Opcode & 0x00FF)
		{
		case 0x0007: // FX07:	Sets VX to the value of the delay timer
			cpu->V[GET_X(cpu->Opcode)] = cpu->Timers.DelayTimer;
			break;
		case 0x000A: // FX0A : A key press is awaited, and then stored in VX
		{
			int keyPressed = -1;
			for (int i = 0; i < 16; i++) {
				if (cpu->Keys[i] == 1) {
					keyPressed = i;
					break;
				}
			}
			if (keyPressed != -1)
				cpu->V[GET_X(cpu->Opcode)] = keyPressed;
			else
				cpu->PC -= 2;
		}
		break;
		case 0x0015: // FX15:	Sets the delay timer to VX
			cpu->Timers.DelayTimer = cpu->V[GET_X(cpu->Opcode)];
			break;
		case 0x0018: // FX18:	Sets the sound timer to VX
			cpu->Timers.SoundTimer = cpu->V[GET_X(cpu->Opcode)];
			break;
		case 0x001E: // FX1E:	Adds VX to I
			cpu->I += cpu->V[GET_X(cpu->Opcode)];
			break;
		case 0x0029: // FX29:	Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font
			cpu->I = cpu->V[GET_X(cpu->Opcode)] * 5; //????
			break;
		case 0x0033: // FX33:	A key press is awaited, and then stored in VX
			cpu->Memory[cpu->I] = cpu->V[GET_X(cpu->Opcode)] / 100;
			cpu->Memory[cpu->I + 1] = (cpu->V[GET_X(cpu->Opcode)] / 10) % 10;
			cpu->Memory[cpu->I + 2] = cpu->V[GET_X(cpu->Opcode)] % 10;
			break;
		case 0x0055: // FX55:	Stores V0 to VX (including VX) in memory starting at address I.
			for (int i = 0; i <= GET_X(cpu->Opcode); i++)
				cpu->Memory[cpu->I + i] = cpu->V[i];
			break;
		case 0x0065: // FX65:	Fills V0 to VX (including VX) with values from memory starting at address I
			for (int i = 0; i <= GET_X(cpu->Opcode); i++)
				cpu->V[i] = cpu->Memory[cpu->I + i];
			break;
		default:
			printf("Unknown opcode: 0x%X\n", cpu->Opcode);
		}
		break;
	default:
		printf("Unknown opcode: 0x%X\n", cpu->Opcode);
	}

	if (cpu->Timers.DelayTimer > 0) {
		cpu->Timers.DelayTimer--;
	}

	if (cpu->Timers.SoundTimer > 0){
		cpu->Timers.SoundTimer--;
	}
}