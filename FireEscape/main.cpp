//
// FIRE ESCAPE
//
// ARCADE GAME FOR HACKED 2016
//

#include <SDL.h>
#include <iostream>

#include "engine.h"
#include "two_d_array.h"

using namespace std;


int main(int argc, char* argv[]) {


	Engine& e = Engine::getInstance();

	if (e.init()) {
		e.run();
	}

	return 0;
}