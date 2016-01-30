#include "main_menu_state.h"

void MainMenuState::update(vector<SDL_Event> input) {

}

void MainMenuState::render(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);

	SDL_Rect r = { 10, 10, 10, 10 };
	SDL_RenderFillRect(renderer, &r);
}