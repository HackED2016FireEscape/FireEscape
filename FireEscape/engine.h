#pragma once

#include <map>
#include "game_state.h"
#include "two_d_array.h"
#include "tile.h"


using namespace std;

class Engine {
public:
	static Engine& getInstance();

	bool init();
	void run();
	void parseLevel();

	enum class StateId
	{
		MAIN_MENU,
		PLACEMENT,
		SIMULATION
	};

	void setState(StateId state);

	void testInit();

	TwoDArray<Tile>& getMap();

private:

	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	SDL_Window* window;
	SDL_Renderer* renderer;

	StateId activeState;
	map<StateId, GameState*> states;

	TwoDArray<Tile> mapData;
	
	Engine();
	~Engine();
};