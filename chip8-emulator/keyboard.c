#include "keyboard.h"

void WaitKeyboard(SDL_Event event, short unsigned *close, CPU *cpu) 
{
	char *key;

	if (SDL_PollEvent(&event)) 
	{
		switch (event.type) 
		{
			case (SDL_QUIT):
				close = 1;
				break;
			case (SDL_KEYDOWN):
				SetKeyboardStatus(KEY_DOWN, event, cpu);
				break;
			case (SDL_KEYUP):
				SetKeyboardStatus(KEY_UP, event, cpu);
				break;
		}
	}
}

void SetKeyboardStatus(unsigned char keyboardStatus, SDL_Event event, CPU *cpu)
{
	char *key = SDL_GetKeyName(event.key.keysym.sym);
	if (strlen(key) > 1)
		return;
	char chr = key[0];
	switch (chr) 
	{
		case SDLK_1:
			cpu->Keys[0] = keyboardStatus;
			break;
		case SDLK_2:
			cpu->Keys[1] = keyboardStatus;
			break;
		case SDLK_3:
			cpu->Keys[2] = keyboardStatus;
			break;
		case SDLK_c:
			cpu->Keys[3] = keyboardStatus;
			break;
		case SDLK_4:
			cpu->Keys[4] = keyboardStatus;
			break;
		case SDLK_5:
			cpu->Keys[5] = keyboardStatus;
			break;
		case SDLK_6:
			cpu->Keys[6] = keyboardStatus;
			break;
		case SDLK_d:
			cpu->Keys[7] = keyboardStatus;
			break;
		case SDLK_7:
			cpu->Keys[8] = keyboardStatus;
			break;
		case SDLK_8:
			cpu->Keys[9] = keyboardStatus;
			break;
		case SDLK_9:
			cpu->Keys[10] = keyboardStatus;
			break;
		case SDLK_e:
			cpu->Keys[11] = keyboardStatus;
			break;
		case SDLK_a:
			cpu->Keys[12] = keyboardStatus;
			break;
		case SDLK_0:
			cpu->Keys[13] = keyboardStatus;
			break;
		case SDLK_b:
			cpu->Keys[14] = keyboardStatus;
			break;
		case SDLK_f:
			cpu->Keys[15] = keyboardStatus;
			break;
	}
}