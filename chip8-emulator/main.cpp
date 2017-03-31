#include "cpu.h"
#include "SDL\include\SDL.h"
#undef _main

static bool success = true;
static SDL_Window* gWindow = NULL;
static SDL_Surface* gScreenSurface = NULL;
static SDL_Event event;
static SDL_Renderer* renderer;
static SDL_Texture* texture;
int VideoStart();
void VideoEnd();

void RenderTo(int pixels[], CPU* cpu)
{
	int limit = W*H;

	for (int i = 0; i < limit; i++)
	{
		int pixel = cpu->Display[i];
		pixels[i] = 0X777775 * pixel;
	}

}

int main(int argc, char* argv[])
{
	VideoStart();

	CPU cpu;
	FILE* rom = fopen(argv[1], "rb");

	Initialization(&cpu, rom);
	fclose(rom);

	int close = 1;
	while (close)
	{
		while (SDL_PollEvent(&event) != 0){
			if (event.type == SDL_QUIT){
				close = 0;
			}
		}

		for (int i = 0; i < 5; i++)
		{
		InstructionCycle(&cpu);
		}

		int pixels[W*H]; RenderTo(pixels, &cpu);
		SDL_UpdateTexture(texture, NULL, pixels, sizeof(int)*W);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
		//SDL_Delay(5);
	}

	VideoEnd();
	return 0;
}

int VideoStart()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, W, H, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}

	renderer = SDL_CreateRenderer(gWindow, -1, 0);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, W, H);

	return success;
}

void VideoEnd()
{
	//Deallocate surface
	SDL_FreeSurface(gScreenSurface);
	gScreenSurface = NULL;

	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}