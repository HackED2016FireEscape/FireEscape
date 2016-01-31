#include "simulation_state.h"
#include "tmxparser.h"
#include "engine.h"
#include "two_d_array.h"
#include "tile.h"

void SimulationState::update(vector<SDL_Event> input) {

}

void SimulationState::render(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
	
	Engine &engine = Engine::getInstance();
	TwoDArray<Tile>& array = engine.getMap();
	
	for (int j = 0; j < array.y; j++) {
		for (int i = 0; i < array.x; i++) {
			Tile currentTile = array[i][j];
			SDL_Texture* texture = engine.getTexture(currentTile.gid + 1);
			
			SDL_Rect texture_rect;
			texture_rect.x = i * engine.TILE_WIDTH;  //the x coordinate
			texture_rect.y = j * engine.TILE_HEIGHT; // the y coordinate
			texture_rect.w = engine.TILE_WIDTH; //the width of the texture
			texture_rect.h = engine.TILE_HEIGHT; //the height of the texture			

			//Render texture to screen
			SDL_RenderCopy(renderer, texture, NULL, &texture_rect);

		}
	}
		
		
		


	

	SDL_Rect r = { 10, 10, 10, 10 };
	SDL_RenderFillRect(renderer, &r);
}
