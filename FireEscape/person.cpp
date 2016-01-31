#include "person.h"
#include "engine.h"
#include <cmath>

Person::Person(Coord<int> p)
:	position(p),
	desiredMove(Direction::IDLE),
	alive(true),
	escaped(false),
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

	vector<Coord<int>> goals;
	for (int i = 0; i < mapData.x; ++i) {
		for (int j = 0; j < mapData.y; ++j) {
			Tile& t = mapData[i][j];
			if (t.isExit || t.isFireExtinguisher) {
				goals.push_back({ i, j });
			}
		}
	}

	vector<Direction> validChoices = {};
	vector<Direction> preferredChoices = {};
	if (state == State::CALM) {
		validChoices.push_back(Direction::IDLE);
	}

	if (!hasFireExtinguisher) {
		if (!mapData.fromCoord(position.operator+({ 0, -1 })).onFire &&
			!mapData.fromCoord(position.operator+({ 0, -2 })).onFire &&
			!mapData.fromCoord(position.operator+({ 0, -3 })).onFire) {
			validChoices.push_back(Direction::UP);
		}
		else if (mapData.fromCoord(position.operator+({ 0, -1 })).onFire) {
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
	}
	if (hasFireExtinguisher && state == State::PANIC) {
		if (!mapData.fromCoord(position.operator+({ 0, -1 })).isExit &&
			!mapData.fromCoord(position.operator+({ 0, -2 })).isExit &&
			!mapData.fromCoord(position.operator+({ 0, -3 })).isExit) {
			validChoices.push_back(Direction::UP);
		}
		else if (mapData.fromCoord(position.operator+({ 0, -1 })).isExit) {
			preferredChoices.push_back(Direction::DOWN);
			state = State::PANIC;
		}
		else {
			state = State::PANIC;
		}
		if (!mapData.fromCoord(position.operator+({ 0, 1 })).isExit &&
			!mapData.fromCoord(position.operator+({ 0, 2 })).isExit &&
			!mapData.fromCoord(position.operator+({ 0, 3 })).isExit) {
			validChoices.push_back(Direction::DOWN);
		}
		else if (mapData.fromCoord(position.operator+({ 0, 1 })).isExit) {
			preferredChoices.push_back(Direction::UP);
			state = State::PANIC;
		}
		else {
			state = State::PANIC;
		}
		if (!mapData.fromCoord(position.operator+({ -1, 0 })).isExit &&
			!mapData.fromCoord(position.operator+({ -2, 0 })).isExit &&
			!mapData.fromCoord(position.operator+({ -3, 0 })).isExit) {
			validChoices.push_back(Direction::LEFT);
		}
		else if (mapData.fromCoord(position.operator+({ -1, 0 })).isExit) {
			preferredChoices.push_back(Direction::RIGHT);
			state = State::PANIC;
		}
		else {
			state = State::PANIC;
		}
		if (!mapData.fromCoord(position.operator+({ 1, 0 })).isExit &&
			!mapData.fromCoord(position.operator+({ 2, 0 })).isExit &&
			!mapData.fromCoord(position.operator+({ 3, 0 })).isExit) {
			validChoices.push_back(Direction::RIGHT);
		}
		else if (mapData.fromCoord(position.operator+({ 1, 0 })).isExit) {
			preferredChoices.push_back(Direction::LEFT);
			state = State::PANIC;
		}
		else if (mapData.fromCoord(position.operator+({ 0, 0 })).isExit) {
			//exit; 
		}
		else {
			state = State::PANIC;
		}
	}

	// targetted walking 
	if (state == State::PANIC && goals.size() != 0) {
		int min = (goals[0] - position).dist();
		Coord<int> GO = goals[0];
		for (auto e : goals) {
			if (min > (e - position).dist()) {
				min = (e - position).dist();
				GO = e;
			}
		}
		// need the left right thing of the position we want to go to 
		if (position.x - GO.x > 0) {
			preferredChoices.push_back(Direction::LEFT);
			preferredChoices.push_back(Direction::LEFT);
			preferredChoices.push_back(Direction::LEFT);
			preferredChoices.push_back(Direction::LEFT);
		}
		else if (position.x - GO.x < 0) {
			preferredChoices.push_back(Direction::RIGHT);
			preferredChoices.push_back(Direction::RIGHT);
			preferredChoices.push_back(Direction::RIGHT);
			preferredChoices.push_back(Direction::RIGHT);
		}
		else if (GO.y - position.y > 0) {
			preferredChoices.push_back(Direction::DOWN);
			preferredChoices.push_back(Direction::DOWN);
			preferredChoices.push_back(Direction::DOWN);
			preferredChoices.push_back(Direction::DOWN);
		}
		else if (position.y - GO.y < 0) {
			preferredChoices.push_back(Direction::UP);
			preferredChoices.push_back(Direction::UP);
			preferredChoices.push_back(Direction::UP);
			preferredChoices.push_back(Direction::UP);
		}
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