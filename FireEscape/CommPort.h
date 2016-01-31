#pragma once

#include <queue>

using namespace std;

class CommPort {
public:
	CommPort();
	~CommPort();
	void update();
	queue<char> getActions();
	bool init();



};


