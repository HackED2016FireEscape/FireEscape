#include "placement_state.h"

void PlacementState::update(vector<SDL_Event> input) {

}

void PlacementState::render(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);

	SDL_Rect r = { 10, 10, 10, 10 };
	SDL_RenderFillRect(renderer, &r);
}