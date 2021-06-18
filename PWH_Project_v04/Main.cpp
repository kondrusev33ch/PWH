#define OLC_PGE_APPLICATION
#include "Visual.h"

int main()
{
	// Close console window
	FreeConsole();

	Visual visual{};

	// Begin main loop 
	if (visual.Construct(1280, 720, 1, 1))
		visual.Start();
	
	return 0;
}