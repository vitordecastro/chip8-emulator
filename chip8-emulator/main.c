#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "SDL\include\SDL.h"
#undef _main

int main(int argc, char* argv[])
{
	Graphic graphic;
	VideoInitialization(&graphic, H, W);

	CPU cpu;
	FILE* rom = fopen(argv[1], "rb");
	Initialization(&cpu, rom);
	fclose(rom);

	short unsigned close = 0;
	while (!close)
	{
		WaitKeyboard(graphic.Event, &close, &cpu);

		for (int i = 0; i < 5; i++)
		{
			InstructionCycle(&cpu);
		}

		VideoUpdate(&graphic, &cpu);
	}

	VideoFinalization(&graphic);
	return 0;
}