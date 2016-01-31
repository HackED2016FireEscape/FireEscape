#pragma once

#include "game_state.h"
#include "coord.h"

class PlacementState : public GameState {
public:
	virtual void update(vector<SDL_Event> input);
	virtual void render(SDL_Renderer* renderer);

private:
	Coord<int> cursorPos = { 0, 0 };
	
	//menu stuff
	bool menuOpen = false;
	SDL_Rect menu_back;
	SDL_Rect menu_top;
	int menu_items = 3;
	SDL_Rect menu_list[3];
	int x = 10;
	int y = 10;
	int h = 200;
	int w = 600;
	int selected = 0; 
};