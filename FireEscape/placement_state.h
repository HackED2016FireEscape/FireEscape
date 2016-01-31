#pragma once

#include "game_state.h"
#include "coord.h"

class PlacementState : public GameState {
public:
	PlacementState();
	virtual void update(vector<SDL_Event> input);
	virtual void render(SDL_Renderer* renderer);

private:
	Coord<int> cursorPos = { 0, 0 };
	int updates = 0;
};