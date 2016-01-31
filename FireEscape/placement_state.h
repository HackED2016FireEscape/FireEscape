#pragma once

#include "game_state.h"
#include "coord.h"

class PlacementState : public GameState {
public:
	PlacementState();
	virtual void update(vector<SDL_Event> input);
	virtual void render(SDL_Renderer* renderer);
	virtual void enter();
	virtual void exit();
	void drawHover(SDL_Renderer * renderer);

private:
	Coord<int> cursorPos = { 0, 0 };
	
	//menu stuff
	bool menuOpen = false;
	SDL_Rect menu_back;
	int menu_items = 5;
	SDL_Rect menu_list[5];
	SDL_Rect menu_img[5];
	SDL_Rect menu_num[5];
	SDL_Texture* textures[5];
	int x = 35;
	int y = 10;
	int h = 200;
	int w = 550;
	int hover = 0; 

	int selected = -1;
	int updates = 0;

};