#include "engine.h"

Engine& Engine::getInstance() {
	static Engine engine;

	return engine;
}

Engine::Engine() {

}