#include <chrono>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <set>

#include "engine.h"
#include "main_menu_state.h"
#include "placement_state.h"
#include "simulation_state.h"

using namespace std;

Engine& Engine::getInstance() {
	static Engine engine;

	return engine;
}

Engine::Engine() {

}

Engine::~Engine() {
	for (auto pair : states) {
		delete pair.second;
	}
}

void Engine::setState(StateId state) {
	activeState = state;
}

void Engine::testInit() {
	mapData.init(3, 3);
	mapData.fill({ false });
	//mapData[0][0] = { true };
	//mapData[1][1] = { true };
	//mapData[2][2] = { true };
	//mapData[3][3] = { true };
	//mapData[4][4] = { true };
	mapData[5][5] = { true };
	//mapData[6][6] = { true };
	//mapData[7][7] = { true };
	//mapData[8][8] = { true };
	//mapData[9][9] = { true };

	people.clear();
	people.push_back({ { 2, 12 } });
	people.push_back({ { 3, 6 } });
	people.push_back({ { 8, 1 } });
	people.push_back({ { 0, 0 } });
	people.push_back({ { 0, 1 } });
	people.push_back({ { 1, 1 } });
	people.push_back({ { 1, 0 } });
	people.push_back({ { 22, 3 } });
	people.push_back({ { 4, 18 } });
}

bool Engine::tileOccupied(Coord<int> position) {
	for (Person& person : people) {
		if (person.position == position) {
			return true;
		}
	}
	return false;
}

TwoDArray<Tile>& Engine::getMap() {
	return mapData;
}

TwoDArray<Tile>& Engine::getItems()
{
	return itemData;
}

vector<Person>& Engine::getPeople() {
	return people;
}

void Engine::processMap() {
	//vector<Coord<int>> checked;
	bool* checked = new bool[mapData.x*mapData.y];
	fill(checked, checked + mapData.x*mapData.y, false);
	queue<Coord<int>> toCheck;

	for (int i = 0; i < mapData.x; ++i) {
		for (int j = 0; j < mapData.y; ++j) {
			if (mapData[i][j].onFire) {
				//cout << i << ", " << j << " is on fire!" << endl;
				mapData[i][j].fireDistance = 0;
				toCheck.push({ i, j });
			}
			else {
				mapData[i][j].fireDistance = -1;
			}
		}
	}

	while (toCheck.size() > 0) {
		Coord<int> current = toCheck.front();
		//cout << "Checking: " << current.x << ", " << current.y << endl;
		toCheck.pop();
		if (checked[current.x * mapData.y + current.y]) {
			continue;
		}

		Coord<int> n = current + Coord<int>{ 0, -1 };
		Coord<int> s = current + Coord<int>{ 0, 1 };
		Coord<int> e = current + Coord<int>{ 1, 0 };
		Coord<int> w = current + Coord<int>{ -1, 0 };

		if (n.y >= 0) {
			if (!checked[n.x * mapData.y + n.y]) {
				mapData.fromCoord(n).fireDistance = mapData.fromCoord(current).fireDistance + 1;
				//cout << "(N) Pushing: " << n.x << ", " << n.y << endl;
				//toCheck.push(n);
			}
		}
		if (s.y < mapData.y) {
			if (!checked[s.x * mapData.y + s.y]) {
				mapData.fromCoord(s).fireDistance = mapData.fromCoord(current).fireDistance + 1;
				//cout << "(S) Pushing: " << s.x << ", " << s.y << endl;
				//toCheck.push(s);
			}
		}
		if (e.x < mapData.x) {
			if (!checked[e.x * mapData.y + e.y]) {
				mapData.fromCoord(e).fireDistance = mapData.fromCoord(current).fireDistance + 1;
				//cout << "(E) Pushing: " << e.x << ", " << e.y << endl;
				//toCheck.push(e);
			}
		}
		if (w.x >= 0) {
			if (!checked[w.x * mapData.y + w.y]) {
				mapData.fromCoord(w).fireDistance = mapData.fromCoord(current).fireDistance + 1;
				//cout << "(W) Pushing: " << w.x << ", " << w.y << endl;
				//toCheck.push(w);
			}
		}
		checked[current.x * mapData.y + current.y] = true;
		//SDL_Delay(100);
	}

	//cout << "Fire Distances!" << endl;
	for (int j = 0; j < mapData.y; ++j) {
		for (int i = 0; i < mapData.x; ++i) {
			//cout << mapData[i][j].fireDistance << " ";
		}
		//cout << endl;
	}
	//cout << "------------------------";

}

bool Engine::init() {
	window = SDL_CreateWindow("~==FireEscape==~", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		return false;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		return false;
	}
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		return false;
	}

	states[StateId::MAIN_MENU] = new MainMenuState{};
	states[StateId::PLACEMENT] = new PlacementState{};
	states[StateId::SIMULATION] = new SimulationState{};
	activeState = StateId::MAIN_MENU;

	// Hard-coded ftw
	textures[AssetId::LOGO] = IMG_LoadTexture(renderer, "res/logo.png");
	textures[AssetId::PRESS_START] = IMG_LoadTexture(renderer, "res/press_start.png");
	loadLevel("./res/untitled.tmx");

	return true;
}

void Engine::run() {
	// Gameloop goes here
	using namespace std::chrono;

	steady_clock::time_point start;
	steady_clock::time_point prevStart = steady_clock::now();

	double t = 0.0;
	double dt = 0.01;

	double accumulator = 0.0;

	bool quit = false;
	SDL_Event e;
	vector<SDL_Event> events;
	while (!quit) {
		start = steady_clock::now();

		steady_clock::duration chronoFrameTime = start - prevStart;

		prevStart = start;

		double ratio = (double) steady_clock::period::num / (double) steady_clock::period::den;
		double frameDuration = double(chronoFrameTime.count()) * ratio;

		accumulator += frameDuration;

		while (accumulator >= dt) {
			// Update goes here
			while (SDL_PollEvent(&e) != 0) {
				events.push_back(e);
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				if (e.type == SDL_KEYDOWN) {
					if (e.key.keysym.sym == SDLK_m) {
						setState(StateId::MAIN_MENU);
					}
					else if (e.key.keysym.sym == SDLK_p) {
						setState(StateId::PLACEMENT);
					}
					else if (e.key.keysym.sym == SDLK_s) {
						setState(StateId::SIMULATION);
					}
				}
			}
			states[activeState]->update(events);
			events.clear();
			// Also probably need to get input from Joystick and buttons here


			t += dt;
			accumulator -= dt;
		}

		double alpha = accumulator / dt;

		// Some interpolation (using alpha) can happen here before we render

		// Render goes here
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);
		states[activeState]->render(renderer);
		SDL_RenderPresent(renderer);
	}
}

void Engine::loadLevel(std::string mapFile) {
	std::string path = "./res/";
	tmxparser::TmxReturn error = tmxparser::parseFromFile(mapFile, &tiledMap, path);

	// Load the images into our texture map.
	if (tiledMap.tilesetCollection.size() == 0) {
		testInit();
		return;
	}

	for (auto it : tiledMap.tilesetCollection[0].tileDefinitions) {
		tmxparser::TmxImage image = it.second.image;

		string imgPath = path + image.source;

		SDL_Texture* texture = IMG_LoadTexture(renderer, imgPath.c_str());
		textures[it.second.id] = texture;

		//Map<std::string, std::string> properties = it.second.propertyMap;

		tileDefault[it.second.id] = {};
	}

	// Now we build our level.	
	int width = tiledMap.layerCollection[0].width;
	int height = tiledMap.layerCollection[0].height;
	mapData.init(width, height);
	std::vector<tmxparser::TmxLayerTile> mapTiles = tiledMap.layerCollection[0].tiles;

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			mapData[i][j] = { false, mapTiles[i + width * j].gid };			
		}
	}	

	// Get the items
	width = tiledMap.layerCollection[0].width;
	height = tiledMap.layerCollection[0].height;
	itemData.init(width, height);
	std::vector<tmxparser::TmxLayerTile> itemTiles = tiledMap.layerCollection[0].tiles;

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			itemData[i][j] = { false, itemTiles[i + width * j].gid };
		}
	}

	people.clear();
	people.push_back({ { 2, 12 } });
	people.push_back({ { 3, 6 } });
	people.push_back({ { 8, 1 } });
}

SDL_Texture* Engine::getTexture(int key) {
	return textures[key];
}