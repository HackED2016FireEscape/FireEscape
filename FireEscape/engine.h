#pragma once

class Engine {
public:
	static Engine& getInstance();

	int test = 5;
private:
	Engine();
};