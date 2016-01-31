#include "person.h"
#include "engine.h"


void Person::decide() {
	TwoDArray<Tile>& map = Engine::getInstance().getMap();

	int direction = (rand() >> 8) % 4;
	if (direction == 0) {
		desiredMove = Direction::UP;
	}
	else if (direction == 1) {
		desiredMove = Direction::DOWN;
	}
	else if (direction == 2) {
		desiredMove = Direction::LEFT;
	}
	else if (direction == 3) {
		desiredMove = Direction::RIGHT;
	}
}