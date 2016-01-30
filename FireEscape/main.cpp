//
// FIRE ESCAPE
//
// ARCADE GAME FOR HACKED 2016
//

#include <SDL.h>
#include <iostream>

#include "engine.h"

using namespace std;


int main(int argc, char* argv[]) {
	SDL_Window* window;

	Engine& e = Engine::getInstance();
	Engine& e2 = Engine::getInstance();

	e.test = 10;

	std::cout << "e2: " << e2.test << std::endl;

	if (e.init()) {
		e.run();
	}

	// Keep this until we have a gameloop running
	SDL_Delay(1000);

	return 0;
}