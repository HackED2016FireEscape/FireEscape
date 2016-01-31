// Please add new params to the end.
#pragma once
struct Tile {
	bool onFire;
	bool isPathable;
	bool isFlammable;
	bool isFireSource;
	bool isExit;
	int gid;
	bool closeToFire;
};