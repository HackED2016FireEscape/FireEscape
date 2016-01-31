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

bool Engine::tileOccupied(Coord<int> position) {
	if (!getItems().fromCoord(position).isPathable) {
		return true;
	}
	for (Person& person : people) {
		if (person.position == position) {
			return true;
		}
	}
	return false;
}

vector<TwoDArray<Tile>*>& Engine::getMap() {
	return mapData;
}

TwoDArray<Tile>& Engine::getItems()
{
	return *mapData.at(itemLocation);
}

vector<Person>& Engine::getPeople() {
	return people;
}

void Engine::processMap() {
	////vector<Coord<int>> checked;
	//TwoDArray<Tile>& items = getItems();

	//bool* checked = new bool[items.x*items.y];
	//fill(checked, checked + items.x*items.y, false);
	//queue<Coord<int>> toCheck;

	//for (int i = 0; i < items.x; ++i) {
	//	for (int j = 0; j < items.y; ++j) {
	//		if (items[i][j].onFire) {
	//			//cout << i << ", " << j << " is on fire!" << endl;
	//			items[i][j].fireDistance = 0;
	//			toCheck.push({ i, j });
	//		}
	//		else {
	//			items[i][j].fireDistance = -1;
	//		}
	//	}
	//}

	//while (toCheck.size() > 0) {
	//	Coord<int> current = toCheck.front();
	//	//cout << "Checking: " << current.x << ", " << current.y << endl;
	//	toCheck.pop();
	//	if (checked[current.x * items.y + current.y]) {
	//		continue;
	//	}

	//	Coord<int> n = current + Coord<int>{ 0, -1 };
	//	Coord<int> s = current + Coord<int>{ 0, 1 };
	//	Coord<int> e = current + Coord<int>{ 1, 0 };
	//	Coord<int> w = current + Coord<int>{ -1, 0 };

	//	if (n.y >= 0) {
	//		if (!checked[n.x * items.y + n.y]) {
	//			items.fromCoord(n).fireDistance = items.fromCoord(current).fireDistance + 1;
	//			//cout << "(N) Pushing: " << n.x << ", " << n.y << endl;
	//			//toCheck.push(n);
	//		}
	//	}
	//	if (s.y < items.y) {
	//		if (!checked[s.x * items.y + s.y]) {
	//			items.fromCoord(s).fireDistance = items.fromCoord(current).fireDistance + 1;
	//			//cout << "(S) Pushing: " << s.x << ", " << s.y << endl;
	//			//toCheck.push(s);
	//		}
	//	}
	//	if (e.x < items.x) {
	//		if (!checked[e.x * items.y + e.y]) {
	//			items.fromCoord(e).fireDistance = items.fromCoord(current).fireDistance + 1;
	//			//cout << "(E) Pushing: " << e.x << ", " << e.y << endl;
	//			//toCheck.push(e);
	//		}
	//	}
	//	if (w.x >= 0) {
	//		if (!checked[w.x * items.y + w.y]) {
	//			items.fromCoord(w).fireDistance = items.fromCoord(current).fireDistance + 1;
	//			//cout << "(W) Pushing: " << w.x << ", " << w.y << endl;
	//			//toCheck.push(w);
	//		}
	//	}
	//	checked[current.x * items.y + current.y] = true;
	//	//SDL_Delay(100);
	//}

	////cout << "Fire Distances!" << endl;
	//for (int j = 0; j < items.y; ++j) {
	//	for (int i = 0; i < items.x; ++i) {
	//		//cout << items[i][j].fireDistance << " ";
	//	}
	//	//cout << endl;
	//}
	////cout << "------------------------";

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
	textures[AssetId::MAIN_MENU_BACKGROUND] = IMG_LoadTexture(renderer, "res/title_file.png");
	textures[AssetId::FIRE1] = IMG_LoadTexture(renderer, "res/fire0.png.png");
	textures[AssetId::FIRE2] = IMG_LoadTexture(renderer, "res/fire1.png.png");
	textures[AssetId::FIRE3] = IMG_LoadTexture(renderer, "res/fire2.png.png");
	textures[AssetId::FIRE4] = IMG_LoadTexture(renderer, "res/fire3.png.png");
	loadLevel("./res/map2.tmx");

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

	for (auto it : tiledMap.tilesetCollection[0].tileDefinitions) {
		tmxparser::TmxImage image = it.second.image;

		string imgPath = path + image.source;

		SDL_Texture* texture = IMG_LoadTexture(renderer, imgPath.c_str());
		textures[it.second.id] = texture;

		// Here we build the default values for each tile.
		tmxparser::TmxPropertyMap_t properties = it.second.propertyMap;
		
		bool onFire = false;
		bool isPathable = true;
		bool isFlammable = false;
		bool isFireSource = false;
		bool isExit = false;
		for (auto value : properties) {		
			if (value.first.compare("onFire") == 0) {
				if (value.second.compare("true") == 0) {
					onFire = true;
				}
			}
			else if (value.first.compare("isPathable") == 0) {
				if (value.second.compare("false") == 0) {
					isPathable = false;
				}
			}
			else if (value.first.compare("isFlammable") == 0) {
				if (value.second.compare("true") == 0) {
					isFlammable = true;
				}
			}
			else if (value.first.compare("isFireSource") == 0) {
				if (value.second.compare("true") == 0) {
					isFireSource = true;
				}
			}	
			else if (value.first.compare("isExit") == 0) {
				if (value.second.compare("true") == 0) {
					isExit = true;
				}
			}
		}
		tileDefault[it.second.id] = { onFire, isPathable, isFlammable, isFireSource, (int)it.second.id, isExit };

	}

	// Now we build our level.	
	auto layers = tiledMap.layerCollection;
	int width = layers[0].width;
	int height = layers[0].height;
	itemData.init(width, height);
	for (int k = 0; k < tiledMap.layerCollection.size(); k++) {
		auto layer = tiledMap.layerCollection[k];
		string layerName = layer.name;

		TwoDArray<Tile>* data = new TwoDArray<Tile>();
		data->init(width, height);

		// We store where we are keeping the items in the data.
		if (layer.name.compare("items") == 0) {
			itemLocation = k;
		}

		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				int id = layer.tiles[i + width * j].gid;

				(*data)[i][j] = tileDefault[id];
			}
		}
		
		mapData.push_back(data);
	}

	people.clear();
	people.push_back({ { 2, 12 } });
	people.push_back({ { 3, 6 } });
	people.push_back({ { 8, 1 } });
}

SDL_Texture* Engine::getTexture(int key) {
	return textures[key];
}