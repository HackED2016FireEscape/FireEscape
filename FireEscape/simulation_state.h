#pragma once

#include "game_state.h"

class SimulationState : public GameState {
public:
	SimulationState();

	virtual void update(vector<SDL_Event> input);
	virtual void render(SDL_Renderer* renderer);

private:
	int updates = 0;
};