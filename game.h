#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>


class AssetManager;
class ColliderComponent;

class Game
{

public:
	Game();
	~Game();

	enum groupLabels : std::size_t
	{
		groupMap,
		groupPlayers,
		groupEnemies,
		groupColliders,
		groupProjectiles
	};

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);


	void handleEvents();
	void update();
	void render();
	void clean();
	bool isrunning() { return isRunning; }

	
	static SDL_Renderer* renderer;
	static SDL_Event event;
	static bool isRunning;
	static SDL_Rect camera;
	static AssetManager *assets;

private:

	int cnt;
	
	SDL_Window* window;


};