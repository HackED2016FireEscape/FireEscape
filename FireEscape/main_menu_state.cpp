#include "main_menu_state.h"
#include "engine.h"

void MainMenuState::update(vector<SDL_Event> input) {

}

void MainMenuState::render(SDL_Renderer* renderer) {
	Uint32 format;
	int access, w, h;

	Engine& e = Engine::getInstance();


	SDL_SetRenderDrawColor(renderer, 0xAA, 0xEE, 0xFF, 0xFF);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);


	SDL_Texture* logo = e.getTexture(Engine::AssetId::LOGO);
	SDL_QueryTexture(logo, &format, &access, &w, &h);
	SDL_Rect dest = { e.SCREEN_WIDTH / 2 - w * 2 , 100, w * 4, h * 4 };
	SDL_RenderCopy(renderer, logo, NULL, &dest);

	logo = e.getTexture(Engine::AssetId::PRESS_START);
	SDL_QueryTexture(logo, &format, &access, &w, &h);
	dest = { e.SCREEN_WIDTH / 2 - w, 400, w * 2, h * 2 };
	SDL_RenderCopy(renderer, logo, NULL, &dest);
}