#include <chrono>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <iostream>

#include "engine.h"
#include "tmxparser.h"

using namespace std;

Engine& Engine::getInstance() {
	static Engine engine;

	return engine;
}

Engine::Engine() {

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
				if (e.type == SDL_QUIT) {
					cout << "Quit! ===================" << endl;
					quit = true;
				}
			}
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
		if (quit) {
			cout << "=======  Should be quitting! ===============" << endl;
		}
	}

	

}

void Engine::parseLevel() {
	tmxparser::TmxMap map;
	tmxparser::TmxReturn error = tmxparser::parseFromFile("./res/dev-csv.tmx", &map, "./res");
}