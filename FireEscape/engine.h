#pragma once

#include <map>
#include "game_state.h"


using namespace std;

class Engine {
public:
	enum class StateId
	{
		MAIN_MENU,
		PLACEMENT,
		SIMULATION
	};

	static Engine& getInstance();

	bool init();
	void run();

	void setState(StateId state);
private:

	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	SDL_Window* window;
	SDL_Renderer* renderer;

	StateId activeState;
	map<StateId, GameState*> states;
	
	Engine();
	~Engine();
};