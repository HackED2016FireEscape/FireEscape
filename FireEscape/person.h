#pragma once

#include "coord.h"

struct Person {
	enum class Direction {
		LEFT, RIGHT, UP, DOWN, IDLE
	};

	void decide();

	Coord<int> position;
	Direction desiredMove;
};