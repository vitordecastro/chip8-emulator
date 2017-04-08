#pragma once
#include "SDL\include\SDL.h"
#include "SDL\include\SDL_keyboard.h"
#include "cpu.h"


enum
{
	KEY_UP = 0,
	KEY_DOWN = 1

};

/* Functions */

void WaitKeyboard(SDL_Event event, short unsigned *close, CPU *cpu);

void SetKeyboardStatus(unsigned char keyboardStatus, SDL_Event event, CPU *cpu);