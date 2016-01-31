// Please add new params to the end.
#pragma once
struct Tile {
	bool onFire;
	bool isPathable;
	bool isFlammable;
	bool isFireSource;
	bool isFireExtinguisher;
	bool isFireAlarm;
	int gid;
	bool isExit;
};