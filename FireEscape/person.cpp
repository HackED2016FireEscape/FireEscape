#include "person.h"
#include "engine.h"


void Person::decide() {
	TwoDArray<Tile>& map = Engine::getInstance().getMap();
	TwoDArray<Tile>& mapData = Engine::getInstance().getMap();

	vector<Direction> validChoices = { Direction::IDLE };
	if (!mapData.fromCoord(position.operator+({ 0, -1 })).onFire) {
		validChoices.push_back(Direction::UP);
	}
	if (!mapData.fromCoord(position.operator+({ 0, 1 })).onFire) {
		validChoices.push_back(Direction::DOWN);
	}
	if (!mapData.fromCoord(position.operator+({ -1, 0 })).onFire) {
		validChoices.push_back(Direction::LEFT);
	}
	if (!mapData.fromCoord(position.operator+({ 1, 0 })).onFire) {
		validChoices.push_back(Direction::RIGHT);
	}
	//if (validChoices.size() == 0) {
	//	desiredMove = Direction::IDLE;
	//	return;
	//}
	int direction = (rand() >> 8) % validChoices.size();
	desiredMove = validChoices[direction];
}