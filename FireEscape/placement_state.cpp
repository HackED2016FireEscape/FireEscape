#include "placement_state.h"
#include "engine.h"
#include "two_d_array.h"
#include "tile.h"

void PlacementState::update(vector<SDL_Event> input) {
}

void PlacementState::render(SDL_Renderer* renderer) {
	Engine& e = Engine::getInstance();
	TwoDArray<Tile>& mapData = e.getMap();

	for (unsigned i = 0; i < mapData.x; ++i) {
		for (unsigned j = 0; j < mapData.y; ++j) {
			Tile& t = mapData[i][j];
			if (t.onFire) {
				SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 0xAA, 0xAA, 0xAA, 0xFF);
			}
			SDL_Rect r = { 21 * (i + 1), 21 * (j + 1), 20, 20 };
			SDL_RenderFillRect(renderer, &r);
		}
	}

	//SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);

	//SDL_Rect r = { 10, 10, 10, 10 };
	//SDL_RenderFillRect(renderer, &r);
}