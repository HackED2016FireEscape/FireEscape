#pragma once

#include <queue>

using namespace std;

class CommPort {
public:
	CommPort();
	void update();
	queue<char> getActions();



};


