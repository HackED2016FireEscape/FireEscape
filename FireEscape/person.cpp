#include "person.h"
#include "engine.h"

Person::Person(Coord<int> p)
:	position(p),
	desiredMove(Direction::IDLE),
	alive(true),
	hasFireExtinguisher(false) {

	static unsigned nextId = 0;
	id = ++nextId;
}

void Person::decide() {
	if (!alive) {
		desiredMove = Direction::IDLE;
		return;
	}
	Engine& e = Engine::getInstance();

	TwoDArray<Tile>& mapData = e.getItems();

	vector<Direction> validChoices = {};
	vector<Direction> preferredChoices = {};
	if (state == State::CALM) {
		validChoices.push_back(Direction::IDLE);
	}
	if (!mapData.fromCoord(position.operator+({ 0, -1 })).onFire &&
		!mapData.fromCoord(position.operator+({ 0, -2 })).onFire &&
		!mapData.fromCoord(position.operator+({ 0, -3 })).onFire) {
		validChoices.push_back(Direction::UP);
	}
	else if(mapData.fromCoord(position.operator+({ 0, -1 })).onFire) {
		preferredChoices.push_back(Direction::DOWN);
		state = State::PANIC;
	}
	else {
		state = State::PANIC;
	}
	if (!mapData.fromCoord(position.operator+({ 0, 1 })).onFire &&
		!mapData.fromCoord(position.operator+({ 0, 2 })).onFire &&
		!mapData.fromCoord(position.operator+({ 0, 3 })).onFire) {
		validChoices.push_back(Direction::DOWN);
	}
	else if (mapData.fromCoord(position.operator+({ 0, 1 })).onFire) {
		preferredChoices.push_back(Direction::UP);
		state = State::PANIC;
	}
	else {
		state = State::PANIC;
	}
	if (!mapData.fromCoord(position.operator+({ -1, 0 })).onFire &&
		!mapData.fromCoord(position.operator+({ -2, 0 })).onFire &&
		!mapData.fromCoord(position.operator+({ -3, 0 })).onFire) {
		validChoices.push_back(Direction::LEFT);
	}
	else if (mapData.fromCoord(position.operator+({ -1, 0 })).onFire) {
		preferredChoices.push_back(Direction::RIGHT);
		state = State::PANIC;
	}
	else {
		state = State::PANIC;
	}
	if (!mapData.fromCoord(position.operator+({ 1, 0 })).onFire &&
		!mapData.fromCoord(position.operator+({ 2, 0 })).onFire &&
		!mapData.fromCoord(position.operator+({ 3, 0 })).onFire) {
		validChoices.push_back(Direction::RIGHT);
	}
	else if (mapData.fromCoord(position.operator+({ 1, 0 })).onFire) {
		preferredChoices.push_back(Direction::LEFT);
		state = State::PANIC;
	}
	else {
		state = State::PANIC;
	}
	if (preferredChoices.size() > 0) {
		int direction = (rand() >> 8) % preferredChoices.size();
		desiredMove = preferredChoices[direction];
	}
	else if (validChoices.size() == 0) {
		desiredMove = Direction::IDLE;
		return;
	}
	else {
		int direction = (rand() >> 8) % validChoices.size();
		desiredMove = validChoices[direction];
	}
}

bool Person::operator==(const Person& other) {
	return id == other.id;
}

bool Person::operator!=(const Person& other) {
	return id != other.id;
}