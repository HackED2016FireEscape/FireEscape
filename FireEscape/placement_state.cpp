#include <cstdlib>
#include <ctime>
#include <iostream>

#include "placement_state.h"
#include "engine.h"
#include "two_d_array.h"
#include "tile.h"

using namespace std;

PlacementState::PlacementState() {
	srand(time(NULL));
}

void PlacementState::update(vector<SDL_Event> input) {
	Engine& e = Engine::getInstance();
	TwoDArray<Tile>& mapData = e.getItems();


	if (!menuOpen) {
		for (auto e : input) {
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_LEFT) {
					cursorPos.x -= 1;
				}
				else if (e.key.keysym.sym == SDLK_h) {
					menuOpen = !menuOpen;
				}
				else if (e.key.keysym.sym == SDLK_RIGHT) {
					cursorPos.x += 1;
				}
				else if (e.key.keysym.sym == SDLK_UP) {
					cursorPos.y -= 1;
				}
				else if (e.key.keysym.sym == SDLK_DOWN) {
					cursorPos.y += 1;
				}
				else if (e.key.keysym.sym == SDLK_a) {
					mapData[cursorPos.x][cursorPos.y].onFire =
						!mapData[cursorPos.x][cursorPos.y].onFire;
				}
			}
		}
		cursorPos.x = cursorPos.x < 0 ? 0 : cursorPos.x;
		cursorPos.x = cursorPos.x > mapData.x - 1 ? mapData.x - 1 : cursorPos.x;
		cursorPos.y = cursorPos.y < 0 ? 0 : cursorPos.y;
		cursorPos.y = cursorPos.y > mapData.y - 1 ? mapData.y - 1 : cursorPos.y;
	}
	else {
		for (auto e : input) {
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_h) {
					menuOpen = !menuOpen; 
				}
				else if (e.key.keysym.sym == SDLK_DOWN) {
					if (selected < 2) {
						selected += 1;
					}
				}
				else if (e.key.keysym.sym == SDLK_UP) {
					if (selected > 0) {
						selected = selected - 1;
					}
				}
			}
		}
	}
}

void PlacementState::render(SDL_Renderer* renderer) {
	Engine& e = Engine::getInstance();
	TwoDArray<Tile>& mapData = e.getItems();

	SDL_Rect r;
	for (int i = 0; i < mapData.x; ++i) {
		for (int j = 0; j < mapData.y; ++j) {
			Tile& t = mapData[i][j];
			if (t.onFire) {
				SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 0xAA, 0xAA, 0xAA, 0xFF);
			}
			r = { 21 * (i + 1), 21 * (j + 1), 20, 20 };
			SDL_RenderFillRect(renderer, &r);
		}
	}

	if (!menuOpen) {
		SDL_SetRenderDrawColor(renderer, 0xFF, 0x66, 0x66, 0xAA);
		r = { 21 * (cursorPos.x + 1) + 5, 21 * (cursorPos.y + 1) + 5, 10, 10 };
		SDL_RenderFillRect(renderer, &r);
	}

	if (menuOpen) {

		menu_back.x = x; 
		menu_back.y = y;
		menu_back.h = h;
		menu_back.w = w;
		SDL_SetRenderDrawColor(renderer, 0x66, 0xFF, 0x66, 0x0A);
		SDL_RenderFillRect(renderer, &menu_back);

		menu_top.x = x; 
		menu_top.y = y; 
		menu_top.h = h / 10; 
		menu_top.w = w; 
		SDL_SetRenderDrawColor(renderer, 0x66, 0x66, 0xFF, 0x0A);
		SDL_RenderFillRect(renderer, &menu_top);
		
		menu_list[0].x = x; 
		menu_list[0].y = 2 * y + 2;
		menu_list[0].h = (h-(h/10))/3;
		menu_list[0].w = w;
		SDL_SetRenderDrawColor(renderer, 0xFF, 0x66, 0x66, 0x0A);
		SDL_RenderFillRect(renderer, &menu_list[0]);

		for (int num = 1; num < menu_items; num++) {
			menu_list[num] = menu_list[num-1];
			menu_list[num].y = menu_list[num].y + menu_list[num].h + 1;
			SDL_RenderFillRect(renderer, &menu_list[num]);
		}

		SDL_SetRenderDrawColor(renderer, 0x66, 0xFF, 0x66, 0x0A);
		SDL_RenderDrawRect(renderer, &menu_list[selected]); 

	}

	//SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);

	//SDL_Rect r = { 10, 10, 10, 10 };
	//SDL_RenderFillRect(renderer, &r);
}