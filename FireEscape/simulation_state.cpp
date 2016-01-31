#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>

#include "simulation_state.h"
#include "tmxparser.h"
#include "engine.h"
#include "two_d_array.h"
#include "tile.h"
#include "engine.h"
#include "two_d_array.h"
#include "tile.h"
#include "coord.h"

using namespace std;

SimulationState::SimulationState() {
	//srand(time(NULL));
}

void SimulationState::update(vector<SDL_Event> input) {
	Engine& e = Engine::getInstance();
	TwoDArray<Tile>& mapData = e.getMap();
	vector<Person>& people = e.getPeople();
	for (auto e : input) {
	}

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
							toBeLit.push_back({ i, j - 1 });
							//mapData[i][j - 1].onFire = true;
						}
						else if (direction == 3 && j < mapData.y) {
							toBeLit.push_back({ i, j + 1 });
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

		e.processMap();

		for (Person& person : people) {
			person.decide();

			if (person.desiredMove == Person::Direction::UP) {
				if (person.position.y > 0) {
					if (!e.tileOccupied(person.position + Coord<int>{0, -1})) {
						person.position += {0, -1};
					}
				}
			}
			if (person.desiredMove == Person::Direction::DOWN) {
				if (person.position.y < mapData.y - 1) {
					if (!e.tileOccupied(person.position + Coord<int>{0, 1})) {
						person.position += {0, 1};
					}
				}
			}
			if (person.desiredMove == Person::Direction::LEFT) {
				if (person.position.x > 0) {
					if (!e.tileOccupied(person.position + Coord<int>{-1, 0})) {
						person.position += {-1, 0};
					}
				}
			}
			if (person.desiredMove == Person::Direction::RIGHT) {
				if (person.position.x < mapData.x - 1) {
					if (!e.tileOccupied(person.position + Coord<int>{1, 0})) {
						person.position += {1, 0};
					}
				}
			}

			if (mapData.fromCoord(person.position).onFire) {
				person.alive = false;
			}
		}
	}
}

void SimulationState::render(SDL_Renderer* renderer) {

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
	
	Engine &engine = Engine::getInstance();
	TwoDArray<Tile>& mapData = engine.getMap();

	// Draw the background tiles
	for (int j = 0; j < mapData.y; j++) {
		for (int i = 0; i < mapData.x; i++) {
			Tile currentTile = mapData[i][j];
			int temp = currentTile.gid;
			SDL_Texture* texture = engine.getTexture(currentTile.gid - 1);
			SDL_Rect texture_rect;
			texture_rect.x = i * engine.TILE_WIDTH;  //the x coordinate
			texture_rect.y = j * engine.TILE_HEIGHT; // the y coordinate
			texture_rect.w = engine.TILE_WIDTH; //the width of the texture
			texture_rect.h = engine.TILE_HEIGHT; //the height of the texture			

			//Render texture to screen
			SDL_RenderCopy(renderer, texture, NULL, &texture_rect);

		}
	}
		

	TwoDArray<Tile>& itemData = engine.getItems();
	// Draw the items
	for (int j = 0; j < itemData.y; j++) {
		for (int i = 0; i < itemData.x; i++) {
			Tile currentTile = itemData[i][j];
			int temp = currentTile.gid;
			SDL_Texture* texture = engine.getTexture(currentTile.gid - 1);
			SDL_Rect texture_rect;
			texture_rect.x = i * engine.TILE_WIDTH;  //the x coordinate
			texture_rect.y = j * engine.TILE_HEIGHT; // the y coordinate
			texture_rect.w = engine.TILE_WIDTH; //the width of the texture
			texture_rect.h = engine.TILE_HEIGHT; //the height of the texture			

												 //Render texture to screen
			SDL_RenderCopy(renderer, texture, NULL, &texture_rect);

		}
	}

	Engine& e = Engine::getInstance();
	//TwoDArray<Tile>& mapData = e.getMap();
	vector<Person>& people = e.getPeople();
	SDL_Texture* fireTex = e.getTexture(5);
	SDL_Texture* personTex = e.getTexture(9);
	int w, h;
	SDL_QueryTexture(fireTex, NULL, NULL, &w, &h);

	SDL_Rect src;
	SDL_Rect dest;
	for (int i = 0; i < mapData.x; ++i) {
		for (int j = 0; j < mapData.y; ++j) {
			Tile& t = mapData[i][j];
			if (t.onFire) {
				//SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
				src = { 0, 0, e.TILE_WIDTH, e.TILE_HEIGHT };
				dest = { e.TILE_WIDTH * i, e.TILE_HEIGHT * j, e.TILE_WIDTH, e.TILE_HEIGHT };
				SDL_RenderCopy(renderer, fireTex, &src, &dest);
			}
			else {
				//SDL_SetRenderDrawColor(renderer, 0xAA, 0xAA, 0xAA, 0xFF);
			}

			SDL_Rect r = { e.TILE_WIDTH * i, e.TILE_HEIGHT * j, e.TILE_WIDTH, e.TILE_HEIGHT };
			SDL_SetRenderDrawColor(renderer, 0xFF - 0x11 * t.fireDistance < 0 ? 0 : 0xFF - 0x11 * t.fireDistance, 0x11 * t.fireDistance > 0xFF ? 0xFF : 0x11 * t.fireDistance, 0x00, 0);

			SDL_RenderDrawRect(renderer, &r);
		}
	}

	for (auto person : people) {
		if (person.alive) {
			src = { 0, 0, e.TILE_WIDTH, e.TILE_HEIGHT };
			dest = { e.TILE_WIDTH * person.position.x, e.TILE_HEIGHT * person.position.y, e.TILE_WIDTH, e.TILE_HEIGHT };
			SDL_RenderCopy(renderer, personTex, &src, &dest);
		}
		else {
		}
		//r = { 21 * (person.position.x + 1) + 5, 21 * (person.position.y + 1) + 5, 10, 10 };
		//SDL_RenderFillRect(renderer, &r);
	}
}
