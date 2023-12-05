#include "game.h"


Game* game = nullptr;

int main(int argc, char* args[])
{
	// Creates a Delta Time ini.
	const int FPS = 60;
	const int frameDelay = 1000 / FPS; 
	Uint32 frameStart;
	int frameTime;

	game = new Game();
	game->init("Paths of Nosgoth", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false);

	while (game->isrunning())
	{
		frameStart = SDL_GetTicks();

		game->handleEvents();
		game->update();
		game->render();
		
		// After 1 frame of the application, time is measured and Delta time is created
		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime)
			SDL_Delay(frameDelay - frameTime);
	}

	game->clean();

	return 0;
} 