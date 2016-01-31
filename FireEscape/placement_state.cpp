#include <cstdlib>
#include <ctime>
#include <iostream>

#include "placement_state.h"
#include "engine.h"
#include "two_d_array.h"
#include "tile.h"

using namespace std;

PlacementState::PlacementState() {
	//srand(time(NULL));

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
					if (selected == -1) {
						mapData[cursorPos.x][cursorPos.y].onFire =
							!mapData[cursorPos.x][cursorPos.y].onFire;
					}
					else if (selected == 0) {
						mapData[cursorPos.x][cursorPos.y].isFireExtinguisher =
							!mapData[cursorPos.x][cursorPos.y].isFireExtinguisher;
					}
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

		// #HARDCORE HARDCODE
		if (cursorPos.x * e.TILE_WIDTH - e.scrollOffset.x < e.TILE_WIDTH) {
			e.scrollOffset.x = cursorPos.x * e.TILE_WIDTH - e.TILE_WIDTH;
		}
		if (cursorPos.x * e.TILE_WIDTH - e.scrollOffset.x > e.SCREEN_WIDTH - 2 * e.TILE_WIDTH) {
			e.scrollOffset.x = cursorPos.x * e.TILE_WIDTH + 2 * e.TILE_WIDTH - e.SCREEN_WIDTH;
		}
		if (cursorPos.y * e.TILE_HEIGHT - e.scrollOffset.y < e.TILE_HEIGHT) {
			e.scrollOffset.y = cursorPos.y * e.TILE_HEIGHT - e.TILE_HEIGHT;
		}
		if (cursorPos.y * e.TILE_HEIGHT - e.scrollOffset.y > e.SCREEN_HEIGHT - 2 * e.TILE_HEIGHT) {
			e.scrollOffset.y = cursorPos.y * e.TILE_HEIGHT + 2 * e.TILE_HEIGHT - e.SCREEN_HEIGHT;
		}
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
					texture_rect.x = i * e.TILE_WIDTH - e.scrollOffset.x;  //the x coordinate
					texture_rect.y = j * e.TILE_HEIGHT - e.scrollOffset.y; // the y coordinate
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
	SDL_Texture* extinguisherTex = e.getTexture(Engine::AssetId::FIRE_EXTINGUISHER);
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
				dest = { e.TILE_WIDTH * i - e.scrollOffset.x, e.TILE_HEIGHT * j - e.scrollOffset.y, e.TILE_WIDTH, e.TILE_HEIGHT };
				SDL_RenderCopy(renderer, fireTex, &src, &dest);
			}
			if (t.isFireExtinguisher) {
				src = { 0, 0, e.TILE_WIDTH, e.TILE_HEIGHT };
				dest = { e.TILE_WIDTH * i - e.scrollOffset.x, e.TILE_HEIGHT * j - e.scrollOffset.y, e.TILE_WIDTH, e.TILE_HEIGHT };
				SDL_RenderCopy(renderer, extinguisherTex, &src, &dest);
			}

			/*SDL_Rect r = { e.TILE_WIDTH * i, e.TILE_HEIGHT * j, e.TILE_WIDTH, e.TILE_HEIGHT };
			SDL_SetRenderDrawColor(renderer, 0xFF - 0x11 * t.fireDistance < 0 ? 0 : 0xFF - 0x11 * t.fireDistance, 0x11 * t.fireDistance > 0xFF ? 0xFF : 0x11 * t.fireDistance, 0x00, 0);

			SDL_RenderDrawRect(renderer, &r);*/
		}
	}

	for (auto person : people) {
		if (person.alive) {
			src = { 0, 0, e.TILE_WIDTH, e.TILE_HEIGHT };
			dest = { e.TILE_WIDTH * person.position.x - e.scrollOffset.x, e.TILE_HEIGHT * person.position.y - e.scrollOffset.y, e.TILE_WIDTH, e.TILE_HEIGHT };
			SDL_RenderCopy(renderer, personTex, &src, &dest);
		}
		else {
		}
		//r = { 21 * (person.position.x + 1) + 5, 21 * (person.position.y + 1) + 5, 10, 10 };
		//SDL_RenderFillRect(renderer, &r);
	}

	if (!menuOpen) {
		SDL_SetRenderDrawColor(renderer, 0x66, 0x66, 0xFF, 0x66);
		r = { e.TILE_WIDTH * cursorPos.x - e.scrollOffset.x, e.TILE_HEIGHT * cursorPos.y - e.scrollOffset.y, e.TILE_WIDTH, e.TILE_HEIGHT };
		SDL_RenderDrawRect(renderer, &r);
		r.x += 1; r.y += 1; r.w -= 2; r.h -= 2;
		SDL_RenderDrawRect(renderer, &r);

		if (selected != -1) {
			SDL_Rect src = {
				0,
				0,
				Engine::getInstance().TILE_WIDTH,
				Engine::getInstance().TILE_HEIGHT
			};
			SDL_Rect dest = {
				cursorPos.x * Engine::getInstance().TILE_WIDTH - Engine::getInstance().scrollOffset.x,
				cursorPos.y * Engine::getInstance().TILE_HEIGHT - Engine::getInstance().scrollOffset.y,
				Engine::getInstance().TILE_WIDTH,
				Engine::getInstance().TILE_HEIGHT
			};
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderCopy(renderer, textures[selected], &src, &dest);
		}
	}

	if (menuOpen) {

		textures[0] = Engine::getInstance().getTexture(Engine::AssetId::FIRE_EXTINGUISHER);
		textures[1] = Engine::getInstance().getTexture(Engine::AssetId::FIRE1);
		textures[2] = Engine::getInstance().getTexture(Engine::AssetId::FIRE2);
		textures[3] = Engine::getInstance().getTexture(Engine::AssetId::FIRE3);
		textures[4] = Engine::getInstance().getTexture(Engine::AssetId::FIRE4);

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
		
		 //menu items
		menu_list[0].x = x; 
		menu_list[0].y = y + (h/10) + 5;
		menu_list[0].h = (h-(h/10))/3;
		menu_list[0].w = w;
		SDL_SetRenderDrawColor(renderer, 160, 177, 187, 0x0A);
		SDL_RenderFillRect(renderer, &menu_list[0]);
		for (int num = 1; num < menu_items; num++) {
			menu_list[num] = menu_list[num-1];
			menu_list[num].y = menu_list[num].y + menu_list[num].h + 5;
			SDL_RenderFillRect(renderer, &menu_list[num]);
			cout << menu_list[num].x << ", " << menu_list[num].y << ", " << menu_list[num].w << ", " << menu_list[num].h << endl;
		}

		for (int i = 0; i < menu_items; i++) {
			menu_img[i] = menu_list[i];
			menu_img[i].x = menu_img[i].x + 5;
			menu_img[i].y = menu_img[i].y + 5;
			menu_img[i].h = menu_img[i].h - 10;
			menu_img[i].w = menu_img[i].h;
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x0A);
			SDL_RenderFillRect(renderer, &menu_img[i]);

			menu_num[i] = menu_img[i];
			menu_num[i].x = (menu_num[i].x * 2) + (10 * 2);
			menu_num[i].y = menu_num[i].y + 5;
			menu_num[i].h = menu_num[i].h - 10;
			menu_num[i].w = menu_num[i].h;
			SDL_RenderFillRect(renderer, &menu_num[i]);

			//SDL_Texture* t = Engine::getInstance().getTexture(Engine::AssetId::FIRE_EXTINGUISHER);
			SDL_Rect dest = {
				menu_img[i].x + 10,
				menu_img[i].y + 8,
				Engine::getInstance().TILE_WIDTH,
				Engine::getInstance().TILE_HEIGHT
			};
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderCopy(renderer, textures[i], NULL, &dest);
		}
		SDL_SetRenderDrawColor(renderer, 160, 177, 187, 0x0A);

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

	menu_img[hover].x = menu_img[hover].x - 3;
	menu_img[hover].y = menu_img[hover].y - 3;
	menu_img[hover].h = menu_img[hover].h + 6;
	menu_img[hover].w = menu_img[hover].w + 6;
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x0A);
	SDL_RenderFillRect(renderer, &menu_img[hover]);

	menu_num[hover].x = menu_num[hover].x - 3;
	menu_num[hover].y = menu_num[hover].y - 3;
	menu_num[hover].h = menu_num[hover].h + 6;
	menu_num[hover].w = menu_num[hover].h;
	SDL_RenderFillRect(renderer, &menu_num[hover]);


	if (hover == selected) {
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x0A);
		SDL_RenderDrawRect(renderer, &menu_list[hover]);
	}

	//SDL_Texture* t = Engine::getInstance().getTexture(Engine::AssetId::FIRE_EXTINGUISHER);
	SDL_Rect src = {
		0,
		0,
		Engine::getInstance().TILE_WIDTH,
		Engine::getInstance().TILE_HEIGHT
	};
	SDL_Rect dest = {
		menu_img[hover].x + 12,
		menu_img[hover].y + 12,
		Engine::getInstance().TILE_WIDTH,
		Engine::getInstance().TILE_HEIGHT
	};
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderCopy(renderer, textures[hover], &src, &dest);
}