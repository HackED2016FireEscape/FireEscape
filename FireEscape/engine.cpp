#include <chrono>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <iostream>

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
	mapData.init(10, 10);
	mapData.fill({ false });
	mapData[0][0] = { true };
	mapData[1][1] = { true };
	mapData[2][2] = { true };
	mapData[3][3] = { true };
	mapData[4][4] = { true };
	mapData[5][5] = { true };
	mapData[6][6] = { true };
	mapData[7][7] = { true };
	mapData[8][8] = { true };
	mapData[9][9] = { true };
}

TwoDArray<Tile>& Engine::getMap() {
	return mapData;
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

	testInit();

	return true;
}

void Engine::run() {
	// Gameloop goes here
	using namespace std::chrono;

	steady_clock::time_point start;
	steady_clock::time_point prevStart = steady_clock::now();

	double t = 0.0;
	double dt = 0.1;

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

		cout << "frameDuration: " << frameDuration << endl;

		accumulator += frameDuration;

		while (accumulator >= dt) {
			cout << "Update!" << endl;
			// Update goes here
			while (SDL_PollEvent(&e) != 0) {
				cout << "Event!" << endl;
				events.push_back(e);
				if (e.type == SDL_QUIT) {
					cout << "Quit! ===================" << endl;
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
			// Also probably need to get input from Joystick and buttons here

			// Delay here so that we don't loop stupidly fast doing nothing
			SDL_Delay(10);

			t += dt;
			accumulator -= dt;
		}

		SDL_Delay(10);
		cout << "Render!" << endl;
		double alpha = accumulator / dt;

		// Some interpolation (using alpha) can happen here before we render

		// Render goes here
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);
		states[activeState]->render(renderer);
		SDL_RenderPresent(renderer);

		if (quit) {
			cout << "=======  Should be quitting! ===============" << endl;
		}
	}

}