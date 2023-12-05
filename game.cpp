#include "game.h"
#include "textureManager.h"
#include "map.h"
#include "components.h"
#include "collision.h"
#include "vector2D.h"
#include "assetManager.h"
#include <sstream>


// Declarations --------------------------------------------------
Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = { 0,0,800,640 };

AssetManager* Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;

auto& Player(manager.addEntity());
auto& label(manager.addEntity());

// End Declarations ----------------------------------------------

// --Although many of the functions and classes in this file should be kinda understood an attempt will be made to go into detail--
Game::Game(){}
Game::~Game(){}

// the initializer take the name of the window and parameters.
// ------This is a main() function call------
void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{

	// set the screen to fullscreen if parameter is true
	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	// check to see if init has already been run and if not then create the window then set the game to isRunning
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		isRunning = true;
	}
	
	// display error in prompt if text fails to ini
	if (TTF_Init() == -1)
	{
		std::cout << "Error: SDL_TTF \n";
	}


	// this is where assets are ini
	assets->AddTexture("terrain", "assets/");
	assets->AddTexture("player", "assets/");
	assets->AddFont("DePixelBreit", "assets/DePixelBreit.ttf", 9);

	// this is where maps are ini
	map = new Map("terrain", 3, 32);
	

	//ECS Logic Below Here -------------------------------------------------------------------|v|
	// ECS is in charge of setting components to classes in an easy streamline

	// Maps
	map->LoadMap("assets", 25, 20);

	// Player
	Player.addComponent<TransformComponent>(800.0f, 640.0f, 32, 32, 4);
	Player.addComponent<SpriteComponent>("player", true);
	Player.addComponent<KeyboardController>();
	Player.addComponent<ColliderComponent>("player");
	Player.addGroup(groupPlayers);

	// Assets


	// UI
	SDL_Color black = { 0, 0, 0, 255 };
	label.addComponent<UILabel>(10, 10, "Test String", "DePixelBreit", black);

	//End ECS Logic

}


// Group Inserts are assigned here: a way to organize class objects 
auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));

// End of Group Inserts ----------------------------------------------------|^|


// This is a list of global PollEvent keypresses. Use a nested Event list for individual control of objects.
// ------This is a main() function call------
void Game::handleEvents()
{
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}

// This updates various variables and classes of the entire game.
// ------This is a main() function call------
void Game::update()
{	
	// Every frame the players collision and position will be updated
	SDL_Rect playerCol = Player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = Player.getComponent<TransformComponent>().position;

	// UI debug 
	std::stringstream ss;
	ss << "Player position: " << playerPos;
	label.getComponent<UILabel>().SetLabelText(ss.str(), "DePixelBreit");


	// defer to manager and update refresh events
	manager.refresh();
	manager.update();
	
	// check the collision of  Player and collider objects (Walls------)
	for (auto& c : colliders)
	{
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol))
		{
			Player.getComponent<TransformComponent>().position = playerPos;
		}
	}

	// check the collision of Player and collider objects (Projectiles---)
	for (auto& p : projectiles)
	{
		if (Collision::AABB(Player.getComponent<ColliderComponent>().collider,
			p->getComponent<ColliderComponent>().collider))
		{
			p->destroy();
		}
	}


	// Player Based Camera -  this will scroll the background tileset around the Player Object with Padding(extra space to move player before camera scrolls)

	// set camera x,y to half the screen width... which should probably be made const and put into another const int...tbd
	camera.x = Player.getComponent<TransformComponent>().position.x - 400;
	camera.y = Player.getComponent<TransformComponent>().position.y - 320;

	
	//  ---------------------------------------------------------------------------------------------------------------
	//	Background Based Camera -   this will scroll the background tileset around the Player Object **UNUSED ATM**	   |
	//																												   |
	//  Vector2D pVel = Player.getComponent<TransformComponent>().velocity;											   |
	//  int pSpeed = Player.getComponent<TransformComponent>().speed;												   |
	//  																											   |
	//  for (auto t : tiles)																						   |
	//  {																											   |
	//  	t->getComponent<TileComponent>().destRect.x += -(pVel.x * pSpeed);										   |
	//  	t->getComponent<TileComponent>().destRect.y += -(pVel.y * pSpeed);										   |
	//  }																											   |
	//  ---------------------------------------------------------------------------------------------------------------


	// Camera Clamp to prevent camera from scrolling off the tileset

	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w)
		camera.x = camera.w;
	if (camera.y > camera.h)
		camera.y = camera.h;

}

// This updates the renderable objects or sprites of the entire game.
// ------This is a main() function call------
void Game::render()
{
	SDL_RenderClear(renderer);
	for (auto& t : tiles)
	{
		t->draw();
	}
	for (auto& c : colliders)
	{
		c->draw();
	}
	for (auto& p : players)
	{
		p->draw();
	}
	for (auto& p : projectiles)
	{
		p->draw();
	}
	label.draw();


	SDL_RenderPresent(renderer);
}

// This cleans up pointers and misc. data that could overflow and then exits the application.
// ------This is a main() function call------
void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

