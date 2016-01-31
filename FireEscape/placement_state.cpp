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
				else if (e.key.keysym.sym == SDLK_b) {
					selected = -1; 
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
					if (hover < menu_items - 1) {
						hover += 1;
					}
				}
				else if (e.key.keysym.sym == SDLK_UP) {
					if (hover > 0) {
						hover = hover - 1;
					}
				}
				else if (e.key.keysym.sym == SDLK_a) {
					selected = hover; 
					menuOpen = !menuOpen;
				}
				else if (e.key.keysym.sym == SDLK_b) {
					selected = -1;
				}
			}
		}
	}
}

void PlacementState::render(SDL_Renderer* renderer) {
	Engine& e = Engine::getInstance();
	vector<TwoDArray<Tile>*>& mapData = e.getMap();
	SDL_Rect r;

	for (auto data : mapData) {
		// Draw the background tiles
		for (int j = 0; j < data->y; j++) {
			for (int i = 0; i < data->x; i++) {
				Tile currentTile = (*data)[i][j];
				int temp = currentTile.gid;
				SDL_Texture* texture = e.getTexture(currentTile.gid - 1);

				if (texture != nullptr) {
					SDL_Rect texture_rect;
					texture_rect.x = i * e.TILE_WIDTH;  //the x coordinate
					texture_rect.y = j * e.TILE_HEIGHT; // the y coordinate
					texture_rect.w = e.TILE_WIDTH; //the width of the texture
					texture_rect.h = e.TILE_HEIGHT; //the height of the texture			

														 //Render texture to screen
					SDL_RenderCopy(renderer, texture, NULL, &texture_rect);
				}

			}
		}
	}
	
	vector<Person>& people = e.getPeople();
	SDL_Texture* fireTex = e.getTexture(33);
	SDL_Texture* personTex = e.getTexture(38);
	//int w, h;
	//SDL_QueryTexture(fireTex, NULL, NULL, &w, &h);

	SDL_Rect src;
	SDL_Rect dest;
	TwoDArray<Tile>& items = e.getItems();
	for (int i = 0; i < items.x; ++i) {
		for (int j = 0; j < items.y; ++j) {
			Tile& t = items[i][j];
			if (t.onFire) {
				//SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
				src = { 0, 0, e.TILE_WIDTH, e.TILE_HEIGHT };
				dest = { e.TILE_WIDTH * i, e.TILE_HEIGHT * j, e.TILE_WIDTH, e.TILE_HEIGHT };
				SDL_RenderCopy(renderer, fireTex, &src, &dest);
			}
			else {
				//SDL_SetRenderDrawColor(renderer, 0xAA, 0xAA, 0xAA, 0xFF);
			}

			/*SDL_Rect r = { e.TILE_WIDTH * i, e.TILE_HEIGHT * j, e.TILE_WIDTH, e.TILE_HEIGHT };
			SDL_SetRenderDrawColor(renderer, 0xFF - 0x11 * t.fireDistance < 0 ? 0 : 0xFF - 0x11 * t.fireDistance, 0x11 * t.fireDistance > 0xFF ? 0xFF : 0x11 * t.fireDistance, 0x00, 0);

			SDL_RenderDrawRect(renderer, &r);*/
		}
	}

	if (!menuOpen) {
		SDL_SetRenderDrawColor(renderer, 0x66, 0x66, 0xFF, 0x66);
		r = { e.TILE_WIDTH * cursorPos.x, e.TILE_HEIGHT * cursorPos.y, e.TILE_WIDTH, e.TILE_HEIGHT };
		SDL_RenderDrawRect(renderer, &r);
	}

	if (menuOpen) {

		menu_back.x = x; 
		menu_back.y = y;
		menu_back.h = h;
		menu_back.w = w;
		//SDL_SetRenderDrawColor(renderer, 0x66, 0xFF, 0x66, 0x0A);
		//SDL_RenderFillRect(renderer, &menu_back);

		// menu title
		/*menu_top.x = x; 
		menu_top.y = y; 
		menu_top.h = h / 10; 
		menu_top.w = w; 
		SDL_SetRenderDrawColor(renderer, 148, 1, 9, 0x0A);
		SDL_RenderFillRect(renderer, &menu_top);*/
		
		// menu items
		menu_list[0].x = x; 
		menu_list[0].y = y + menu_top.h + 5;
		menu_list[0].h = (h-(h/10))/3;
		menu_list[0].w = w;
		SDL_SetRenderDrawColor(renderer, 160, 177, 187, 0x0A);
		SDL_RenderFillRect(renderer, &menu_list[0]);
		for (int num = 1; num < menu_items; num++) {
			menu_list[num] = menu_list[num-1];
			menu_list[num].y = menu_list[num].y + menu_list[num].h + 5;
			SDL_RenderFillRect(renderer, &menu_list[num]);
		}

		// menu hover
		drawHover(renderer);

		// menu selection
		if (selected != -1 && hover != selected) {
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x0A);
			SDL_RenderDrawRect(renderer, &menu_list[selected]);
		}
		


	}

	//SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);

	//SDL_Rect r = { 10, 10, 10, 10 };
	//SDL_RenderFillRect(renderer, &r);
}



void PlacementState::drawHover(SDL_Renderer* renderer) {
	menu_list[hover].x = menu_list[hover].x - 25;
	menu_list[hover].y = menu_list[hover].y - 3;
	menu_list[hover].h = menu_list[hover].h + 6;
	menu_list[hover].w = menu_list[hover].w + 50;
	SDL_RenderFillRect(renderer, &menu_list[hover]);

	SDL_SetRenderDrawColor(renderer, 144, 157, 165, 0x0A);
	SDL_RenderDrawRect(renderer, &menu_list[hover]);

	if (hover == selected) {
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x0A);
		SDL_RenderDrawRect(renderer, &menu_list[hover]);
	}
}