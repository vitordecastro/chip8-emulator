#include "graphic.h"

int VideoInitialization(Graphic *graphic, int height, int width)
{
	graphic->Success = 1;
	graphic->Window = NULL;
	graphic->ScreenSurface = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		graphic->Success = 0;
	}
	else
	{
		//Create window
		graphic->Window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width * 10, height * 10, SDL_WINDOW_SHOWN);
		if (graphic->Window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			graphic->Success = 0;
		}
		else
		{
			//Get window surface
			graphic->ScreenSurface = SDL_GetWindowSurface(graphic->Window);
		}
	}

	graphic->Renderer = SDL_CreateRenderer(graphic->Window, -1, 0);
	graphic->Texture = SDL_CreateTexture(graphic->Renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);

	return graphic->Success;
}

int VideoUpdate(Graphic *graphic, CPU *cpu)
{
	int pixels[W*H];
	int limit = W*H;

	for (int i = 0; i < limit; i++)
	{
		int pixel = cpu->Display[i];
		pixels[i] = 0X777775 * pixel;
	}

	SDL_UpdateTexture(graphic->Texture, NULL, pixels, sizeof(int)*W);
	SDL_RenderCopy(graphic->Renderer, graphic->Texture, NULL, NULL);
	SDL_RenderPresent(graphic->Renderer);
	SDL_Delay(5);
}

int VideoFinalization(Graphic *graphic)
{
	//Deallocate surface
	SDL_FreeSurface(graphic->ScreenSurface);
	graphic->ScreenSurface = NULL;

	//Destroy window
	SDL_DestroyWindow(graphic->Window);
	graphic->Window = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

