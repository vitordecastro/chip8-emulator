#pragma once
#include <stdio.h>
#include <stdlib.h>

/* Display Resolution */
#define W 64
#define H 32

//First Address to read in memory
#define START_RAM 0x200

//RAND_MAX to rand function
#define RAND_MAX 255

/* Fields from the opcode. */

#define GET_N(opcode)  (opcode & 0x000F) >> 0
#define GET_Y(opcode)  (opcode & 0x00F0) >> 4
#define GET_X(opcode)  (opcode & 0x0F00) >> 8
#define GET_NN(opcode) (opcode & 0x00FF) >> 0
#define GET_NNN(opcode)(opcode & 0x0FFF) >> 0 

/* CHIP 8 CPU Structure  */
typedef struct CPU
{
	//RAM Memory (4KB memory) 
	unsigned char Memory[0x1000];

	//Registers V(0 to F)
	unsigned char V[0x10];

	//Register I (Index)
	unsigned int I;

	//Program Counter
	unsigned int PC;

	//Stack Pointer
	unsigned char SP;

	//Stack with 16 layers
	unsigned char Stack[0x10];

	//Timers (Delay and Sound)
	struct Timers
	{
		unsigned char DelayTimer;
		unsigned char SoundTimer;
	}Timers;

	//Register that contains current opcode
	unsigned short Opcode;

	//Registers that contains key states (0 to F)
	unsigned char Keys[16];

	//Display based on W and H
	unsigned char Display[W*H];

} CPU;

/* CPU Functions */

void Initialization(CPU* cpu, FILE* rom);
void LoadROM(CPU* cpu, FILE* rom);
void LoadFontSets(CPU* cpu);
void InstructionCycle(CPU* cpu);

/* Others */

//Sprites - Fonts(0 to F)
static const unsigned char sprites[80] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};