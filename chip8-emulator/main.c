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

	int close = 0;
	while (!close)
	{
		while (SDL_PollEvent(&graphic.Event) != 0){
			if (&graphic.Event.type == SDL_QUIT){
				close = 1;
			}
		}

		for (int i = 0; i < 5; i++)
		{
			InstructionCycle(&cpu);
		}

		VideoUpdate(&graphic, &cpu);
	}

	VideoFinalization(&graphic);

	return 0;
}