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
	Engine& engine = Engine::getInstance();
	TwoDArray<Tile>& mapData = engine.getItems();
	vector<Person>& people = engine.getPeople();
	const Uint8* state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_LEFT]) {
		engine.scrollOffset.x -= 3;
	}
	if (state[SDL_SCANCODE_RIGHT]) {
		engine.scrollOffset.x += 3;
	}
	if (state[SDL_SCANCODE_UP]) {
		engine.scrollOffset.y -= 3;
	}
	if (state[SDL_SCANCODE_DOWN]) {
		engine.scrollOffset.y += 3;
	}

	if (engine.scrollOffset.x < 0) {
		engine.scrollOffset.x = 0;
	}
	if (engine.scrollOffset.x > mapData.x * engine.TILE_WIDTH - engine.SCREEN_WIDTH) {
		engine.scrollOffset.x = mapData.x * engine.TILE_WIDTH - engine.SCREEN_WIDTH;
	}
	if (engine.scrollOffset.y < 0) {
		engine.scrollOffset.y = 0;
	}
	if (engine.scrollOffset.y > mapData.y * engine.TILE_HEIGHT - engine.SCREEN_HEIGHT) {
		engine.scrollOffset.y = mapData.y * engine.TILE_HEIGHT - engine.SCREEN_HEIGHT;
	}

	for (auto e : input) {
		if (e.type == SDL_KEYDOWN) {
			/*if (e.key.keysym.sym == SDLK_LEFT) {
				engine.scrollOffset.x -= 1;
			}
			else if (e.key.keysym.sym == SDLK_RIGHT) {
				engine.scrollOffset.x += 1;
			}
			else if (e.key.keysym.sym == SDLK_UP) {
				engine.scrollOffset.y -= 1;
			}
			else if (e.key.keysym.sym == SDLK_DOWN) {
				engine.scrollOffset.y += 1;
			}*/
		}
	}

	if (simulate) {
		++updates;
		int chance;
		vector<Coord<int>> toBeLit;
		if (updates > 60) {
			updates = 0;
			if (timerStarted) {
				firefighterResponseTime -= rate;
			}
			if (firefighterResponseTime < 0) {
				reset();
				Engine& e = Engine::getInstance();
				Engine::getInstance().currentLevel++;
				if (Engine::getInstance().currentLevel > 4) {
					Engine::getInstance().setState(Engine::StateId::MAIN_MENU);
					Engine::getInstance().currentLevel = 0;
				}
				else {
					Engine::getInstance().loadLevel(Engine::getInstance().levels[e.currentLevel]);
					Engine::getInstance().setState(Engine::StateId::PLACEMENT);
				}
			}
			bool allDead = true;
			for (auto p : people) {
				if (p.alive) {
					allDead = false;
				}
			}
			if (allDead) {
				reset();
				Engine& e = Engine::getInstance();
				e.currentLevel = 0;
				e.setState(Engine::StateId::MAIN_MENU);
			}
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
					if (t.isFireSource) {
						chance = (rand() >> 4) % 100;
						if (chance > 75) {
							t.onFire = true;
							timerStarted = true;
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

			engine.processMap();

			for (Person& person : people) {
				person.decide();

				if (person.desiredMove == Person::Direction::UP) {
					if (person.position.y > 0) {
						if (!engine.tileOccupied(person.position + Coord<int>{0, -1})) {
							person.position += {0, -1};
						}
					}
				}
				if (person.desiredMove == Person::Direction::DOWN) {
					if (person.position.y < mapData.y - 1) {
						if (!engine.tileOccupied(person.position + Coord<int>{0, 1})) {
							person.position += {0, 1};
						}
					}
				}
				if (person.desiredMove == Person::Direction::LEFT) {
					if (person.position.x > 0) {
						if (!engine.tileOccupied(person.position + Coord<int>{-1, 0})) {
							person.position += {-1, 0};
						}
					}
				}
				if (person.desiredMove == Person::Direction::RIGHT) {
					if (person.position.x < mapData.x - 1) {
						if (!engine.tileOccupied(person.position + Coord<int>{1, 0})) {
							person.position += {1, 0};
						}
					}
				}

				if (mapData.fromCoord(person.position).isFireExtinguisher) {
					mapData.fromCoord(person.position).isFireExtinguisher = false;
					person.hasFireExtinguisher = true;
				}
				if (person.hasFireExtinguisher) {
					mapData.fromCoord(person.position).onFire = false;
				}
				if (mapData.fromCoord(person.position).onFire) {
					person.alive = false;
				}
				if (mapData.fromCoord(person.position).isExit) {
					person.escaped = true;
					rate = 3;
				}
			}
		}

	}
}

void SimulationState::render(SDL_Renderer* renderer) {

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
	
	Engine &engine = Engine::getInstance();
	vector<TwoDArray<Tile>*>& mapData = engine.getMap();


	for (auto data : mapData) {
		// Draw the background tiles
		for (int j = 0; j < data->y; j++) {
			for (int i = 0; i < data->x; i++) {
				Tile currentTile = (*data)[i][j];
				int temp = currentTile.gid;
				SDL_Texture* texture = engine.getTexture(currentTile.gid - 1);

				if (texture != nullptr) {
					SDL_Rect texture_rect;
					texture_rect.x = i * engine.TILE_WIDTH - engine.scrollOffset.x;  //the x coordinate
					texture_rect.y = j * engine.TILE_HEIGHT - engine.scrollOffset.y; // the y coordinate
					texture_rect.w = engine.TILE_WIDTH; //the width of the texture
					texture_rect.h = engine.TILE_HEIGHT; //the height of the texture			

					//Render texture to screen
					SDL_RenderCopy(renderer, texture, NULL, &texture_rect);
				}
			}
		}
	}

	Engine& e = Engine::getInstance();
	//TwoDArray<Tile>& mapData = e.getMap();
	vector<Person>& people = e.getPeople();
	SDL_Texture* fireTex = e.getTexture(33);
	SDL_Texture* personTex = e.getTexture(38);
	SDL_Texture* extinguisherTex = e.getTexture(Engine::AssetId::FIRE_EXTINGUISHER);
	int w, h;
	SDL_QueryTexture(fireTex, NULL, NULL, &w, &h);

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



	SDL_Rect rBkg = { e.SCREEN_WIDTH / 2 - 100, 10, 200, 15 };
	SDL_Rect rTime = { e.SCREEN_WIDTH / 2 - 99, 11, 198 * (double)firefighterResponseTime / 60, 13 };

	SDL_SetRenderDrawColor(renderer, 0xAA, 0xAA, 0x33, 0xFF);
	SDL_RenderFillRect(renderer, &rBkg);
	SDL_SetRenderDrawColor(renderer, 0x33, 0x33, 0xBB, 0xFF);
	SDL_RenderFillRect(renderer, &rTime);

	SDL_Texture* truck = e.getTexture(Engine::AssetId::FIRE_TRUCK);

	dest = {
		e.SCREEN_WIDTH / 2 - 90- e.TILE_WIDTH * 2,
		-5,
		e.TILE_WIDTH,
		e.TILE_WIDTH
	};

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderCopy(renderer, truck, NULL, &dest);
}

void SimulationState::start() {
	simulate = true;
}

void SimulationState::reset() {
	updates = 0;
	firefighterResponseTime = 60;
	rate = 1;
	timerStarted = false;
	simulate = false;
}

void SimulationState::enter() {

}

void SimulationState::exit() {

}