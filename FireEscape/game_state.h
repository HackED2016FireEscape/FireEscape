#pragma once

#include <vector>
#include <SDL.h>

using namespace std;

class GameState {
public:
	virtual void update(vector<SDL_Event> input) = 0;
	virtual void render(SDL_Renderer* renderer) = 0;
};