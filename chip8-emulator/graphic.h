#pragma once
#include "SDL\include\SDL.h"
#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
/*Struct with all necessary sdl types to display the game */
typedef struct Graphic {

	int Success;
	SDL_Window* Window;
	SDL_Surface* ScreenSurface;
	SDL_Event Event;
	SDL_Renderer* Renderer;
	SDL_Texture* Texture;

} Graphic;

/* Functions */

int VideoInitialization(Graphic *graphic, int height, int width);
int VideoUpdate(Graphic *graphic, CPU *cpu);
int VideoFinalization(Graphic *graphic);
