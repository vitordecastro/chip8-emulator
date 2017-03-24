#include "cpu.h"
#include "SDL\include\SDL.h"
#undef _main

int main(int argc, char* argv[])
{
	CPU CPU;
	FILE ROM;

	Initialization(&CPU, &ROM);

	return 0;
}
