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

	int itemLocation;

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

	enum AssetId {
		LOGO = -2,
		PRESS_START = -3
	};

	void setState(StateId state);

	SDL_Texture* getTexture(int key);

	bool tileOccupied(Coord<int> position);

	vector<TwoDArray<Tile>*>& getMap();
	TwoDArray<Tile>& getItems();
	vector<Person>& getPeople();

	void processMap();

	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

private:


	SDL_Window* window;
	SDL_Renderer* renderer;

	StateId activeState;
	map<StateId, GameState*> states;
	map<int, SDL_Texture*> textures;
	map<int, Tile> tileDefault;

	TwoDArray<Tile> itemData;
	vector<TwoDArray<Tile>*> mapData;
	tmxparser::TmxMap tiledMap;
	vector<Person> people;
	
	Engine();
	~Engine();
};