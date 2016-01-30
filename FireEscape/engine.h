#pragma once

class Engine {
public:
	static Engine& getInstance();

	bool init();
	void run();
	void parseLevel();

	int test = 5;

private:
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	SDL_Window* window;
	SDL_Renderer* renderer;

	Engine();
};