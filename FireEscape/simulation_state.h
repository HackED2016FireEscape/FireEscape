#pragma once

#include "game_state.h"

class SimulationState : public GameState {
public:
	SimulationState();

	virtual void update(vector<SDL_Event> input);
	virtual void render(SDL_Renderer* renderer);
	virtual void enter();
	virtual void exit();

	void start();
	void reset();

private:
	int updates = 0;
	int firefighterResponseTime = 60;
	int rate = 1;
	bool timerStarted = false;
	bool simulate = true;
};