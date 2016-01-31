#pragma once

#include "coord.h"

struct Person {
	enum class Direction {
		LEFT, RIGHT, UP, DOWN, IDLE
	};
	enum class State {
		CALM, PANIC
	};

	Person(Coord<int> p);
	void decide();
	bool operator==(const Person& other);
	bool operator!=(const Person& other);

	unsigned id;
	Coord<int> position;
	Direction desiredMove;
	State state = State::CALM;
	bool alive;
};