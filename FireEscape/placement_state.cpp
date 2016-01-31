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
	TwoDArray<Tile>& mapData = e.getMap();
	for (auto e : input) {
		if (e.type == SDL_KEYDOWN) {
			if (e.key.keysym.sym == SDLK_LEFT) {
				cursorPos.x -= 1;
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
	cursorPos.x = cursorPos.x > mapData.x-1 ? mapData.x-1 : cursorPos.x;
	cursorPos.y = cursorPos.y < 0 ? 0 : cursorPos.y;
	cursorPos.y = cursorPos.y > mapData.y-1 ? mapData.y-1 : cursorPos.y;

	++updates;
	int chance;
	vector<Coord<int>> toBeLit;
	if (updates > 60) {
		updates = 0;

		for (int i = 0; i < mapData.x; ++i) {
			for (int j = 0; j < mapData.y; ++j) {
				Tile& t = mapData[i][j];
				if (t.onFire) {
					chance = (rand() >> 4) % 100;
					//cout << "random output: " << chance << endl;
					if (chance > 50) {
						int direction = (rand() >> 8) % 4;
						if (direction == 0 && i > 0) {
							toBeLit.push_back({ i - 1, j });
							//mapData[i - 1][j].onFire = true;
						}
						else if (direction == 1 && i < mapData.x) {
							toBeLit.push_back({ i + 1, j });
							//mapData[i + 1][j].onFire = true;
						}
						else if (direction == 2 && j > 0) {
							toBeLit.push_back({ i, j - 1});
							//mapData[i][j - 1].onFire = true;
						}
						else if (direction == 3 && j < mapData.y) {
							toBeLit.push_back({ i, j + 1});
							//mapData[i][j + 1].onFire = true;
						}
					}
				}
			}
		}
		for (auto coord : toBeLit) {
			if (coord.x < 0 || coord.x >= mapData.x || coord.y < 0 || coord.y >= mapData.y) {
				continue;
			}
			mapData[coord.x][coord.y].onFire = true;
		}
	}
}

void PlacementState::render(SDL_Renderer* renderer) {
	Engine& e = Engine::getInstance();
	TwoDArray<Tile>& mapData = e.getMap();

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

	SDL_SetRenderDrawColor(renderer, 0xFF, 0x66, 0x66, 0xAA);
	r = { 21 * (cursorPos.x + 1) + 5, 21 * (cursorPos.y + 1) + 5, 10, 10 };
	SDL_RenderFillRect(renderer, &r);
	
	//SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);

	//SDL_Rect r = { 10, 10, 10, 10 };
	//SDL_RenderFillRect(renderer, &r);
}