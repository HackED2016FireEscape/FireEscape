#pragma once

#include <map>
#include <queue>
#include <mutex>

#include "game_state.h"
#include "two_d_array.h"
#include "tile.h"
#include "tmxparser.h"
#include "person.h"
#include <SDL_mixer.h>
#include "CommPort.h"


using namespace std;

extern CommPort* port;

class Engine {
public:
	const int TILE_WIDTH = 32;
	const int TILE_HEIGHT = 32;

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
		PRESS_START = -3,
		MAIN_MENU_BACKGROUND = -4,
		FIRE1 = -5,
		FIRE2 = -6,
		FIRE3 = -7,
		FIRE4 = -8,
		FIRE_EXTINGUISHER = -9,
		FIRE_TRUCK = -10
	};

	enum SoundId {
		MENU_MUSIC,
		FIRE,
		SIREN,
		CLICK1,
		CLICK2
	};

	void setState(StateId state);

	SDL_Texture* getTexture(int key);

	bool tileOccupied(Coord<int> position);

	vector<TwoDArray<Tile>*>& getMap();
	TwoDArray<Tile>& getItems();
	vector<Person>& getPeople();

	void processMap();
	

	queue<char>& getActions();

	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	mutex actionMutex;
	
	Coord<int> scrollOffset = { 0, 0 };
	int top;
	int bottom;
	int left;
	int right;
	int peopleCount;

	map<int, Mix_Music*> music;
	map<int, Mix_Chunk*> sounds;

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
	queue<char> actions;
	thread commThread;

	Engine();
	~Engine();
};