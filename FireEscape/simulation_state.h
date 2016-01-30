#pragma once

#include "game_state.h"

class SimulationState : public GameState {
public:
	virtual void update(vector<SDL_Event> input);
	virtual void render(SDL_Renderer* renderer);
};