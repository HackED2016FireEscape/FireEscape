#pragma once

#include <map>
#include "game_state.h"
#include "two_d_array.h"
#include "tile.h"
#include "tmxparser.h"
#include "person.h"


using namespace std;

class Engine {
public:
	const int TILE_WIDTH = 16;
	const int TILE_HEIGHT = 16;

	static Engine& getInstance();

	bool init();
	void run();
	void loadLevel(std::string mapFile);

	enum class StateId
	{
		MAIN_MENU,
		PLACEMENT,
		SIMULATION
	};

	void setState(StateId state);

	void testInit();

	SDL_Texture* getTexture(int key);

	bool tileOccupied(Coord<int> position);

	TwoDArray<Tile>& getMap();
	tmxparser::TmxMap& getTiledMap();
	vector<Person>& getPeople();

private:

	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	SDL_Window* window;
	SDL_Renderer* renderer;

	StateId activeState;
	map<StateId, GameState*> states;
	map<int, SDL_Texture*> textures;

	TwoDArray<Tile> mapData;
	tmxparser::TmxMap tiledMap;
	vector<Person> people;
	
	Engine();
	~Engine();
};