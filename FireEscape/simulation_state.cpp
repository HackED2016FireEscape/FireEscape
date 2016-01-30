#include "simulation_state.h"

void SimulationState::update(vector<SDL_Event> input) {

}

void SimulationState::render(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);

	SDL_Rect r = { 10, 10, 10, 10 };
	SDL_RenderFillRect(renderer, &r);
}