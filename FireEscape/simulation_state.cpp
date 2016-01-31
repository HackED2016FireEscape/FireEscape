#include <cstdlib>
#include <ctime>
#include <iostream>

#include "simulation_state.h"
#include "engine.h"
#include "two_d_array.h"
#include "tile.h"
#include "coord.h"

SimulationState::SimulationState() {
	srand(time(NULL));
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

		for (Person& person : people) {
			person.decide();
			if (person.desiredMove == Person::Direction::UP) {
				cout << "UP!" << endl;
				cout << "before: " << person.position.y << endl;
				person.position.y -= 1;
				cout << "after: " << person.position.y << endl;
			}
			if (person.desiredMove == Person::Direction::DOWN) {
				cout << "DOWN!";
				person.position.y += 1;
			}
			if (person.desiredMove == Person::Direction::LEFT) {
				cout << "LEFT!";
				person.position.x -= 1;
			}
			if (person.desiredMove == Person::Direction::RIGHT) {
				cout << "RIGHT!";
				person.position.x += 1;
			}
		}
	}
}

void SimulationState::render(SDL_Renderer* renderer) {
	Engine& e = Engine::getInstance();
	TwoDArray<Tile>& mapData = e.getMap();
	vector<Person>& people = e.getPeople();

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

	for (auto person : people) {
		SDL_SetRenderDrawColor(renderer, 0xAA, 0x55, 0x55, 0xFF);
		r = { 21 * (person.position.x + 1) + 5, 21 * (person.position.y + 1) + 5, 10, 10 };
		SDL_RenderFillRect(renderer, &r);
	}
}