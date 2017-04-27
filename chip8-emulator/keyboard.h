#pragma once
#include "SDL\include\SDL.h"
#include "SDL\include\SDL_keyboard.h"
#include "cpu.h"


enum
{
	KEY_UP = 1,
	KEY_DOWN = 0

};

/* Functions */

void WaitKeyboard(SDL_Event event, short unsigned *close, CPU *cpu);

void SetKeyboardStatus(unsigned char keyboardStatus, SDL_Event event, CPU *cpu);