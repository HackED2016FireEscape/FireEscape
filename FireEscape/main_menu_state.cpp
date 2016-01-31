#include "main_menu_state.h"
#include "engine.h"

void MainMenuState::update(vector<SDL_Event> input) {
	Engine& e = Engine::getInstance();

	if (!e.getActions().empty()) {
		if (e.actionMutex.try_lock()) {   // only increase if currently not locked.
			queue<char>& actions = e.getActions();

			for (int i = 0; i < actions.size(); i++) {
				char action = actions.front();
				actions.pop();

				switch (action) {
				case '6':
					//S
					e.setState(Engine::StateId::PLACEMENT);
					break;

				}
			}

			e.actionMutex.unlock();
		}
	}
}

void MainMenuState::render(SDL_Renderer* renderer) {
	Uint32 format;
	int access, w, h;

	Engine& e = Engine::getInstance();

	SDL_SetRenderDrawColor(renderer, 0xAA, 0xEE, 0xFF, 0xFF);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	SDL_Texture* background = e.getTexture(Engine::AssetId::MAIN_MENU_BACKGROUND);
	SDL_QueryTexture(background, &format, &access, &w, &h);
	SDL_Rect dest = { 0 , 0, w * 1.5, h * 1.5 };
	SDL_RenderCopy(renderer, background, NULL, &dest);

	SDL_Texture* logo = e.getTexture(Engine::AssetId::LOGO);
	SDL_QueryTexture(logo, &format, &access, &w, &h);
	dest = { e.SCREEN_WIDTH / 2 - w * 2 , 100, w * 4, h * 4 };
	SDL_RenderCopy(renderer, logo, NULL, &dest);

	logo = e.getTexture(Engine::AssetId::PRESS_START);
	SDL_QueryTexture(logo, &format, &access, &w, &h);
	dest = { e.SCREEN_WIDTH / 2 - w, 400, w * 2, h * 2 };
	SDL_RenderCopy(renderer, logo, NULL, &dest);
}

void MainMenuState::enter() {
	//Mix_PlayMusic(Engine::getInstance().music[Engine::SoundId::MENU_MUSIC], -1);
};

void MainMenuState::exit() {
	//Mix_HaltMusic();
};
