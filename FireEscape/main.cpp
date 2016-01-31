//
// FIRE ESCAPE
//
// ARCADE GAME FOR HACKED 2016
//

#include <SDL.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "engine.h"
#include "two_d_array.h"

#include "SDL_ttf.h"

using namespace std;


int main(int argc, char* argv[]) {
	srand(time(NULL));

	TTF_Init();

	Engine& e = Engine::getInstance();

	if (e.init()) {
		e.run();
	}

	TTF_Quit();

	return 0;
}