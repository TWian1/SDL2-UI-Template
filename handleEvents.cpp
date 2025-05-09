#include "HandleEvents.hpp"


void handleEvents(bool& gameloop, std::vector<button>& buttons, SDL_Renderer* renderer) {

	//Event listener
	SDL_Event e;

	//Event loop
	while (SDL_PollEvent(&e)) {
		switch (e.type) {

			//When Quit exit game loop
		case SDL_QUIT:
			gameloop = false;
			break;

			//If Escape pressed exit game loop
		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_ESCAPE) gameloop = false;
			break;

			// LMB press mechanics
		case SDL_MOUSEBUTTONDOWN:
			if (e.button.button == SDL_BUTTON_LEFT) {
				//If clicking on button call button click function
				for (auto& but : buttons) {
					if (but.visible == false || but.clickable == false) continue;
					if (but.click_test({ (float)e.motion.x, (float)e.motion.y }, false)) {
						but.pressed = true;
						but.func();
					}
				}
			}
			break;

			// LMB release mechanics
		case SDL_MOUSEBUTTONUP:
			//Unset pressed state for all buttons
			if (e.button.button == SDL_BUTTON_LEFT) {
				for (auto& but : buttons) { but.pressed = false; }
			}
			break;

			//Mouse movement mechanics
		case SDL_MOUSEMOTION:
			//If mouse hovers over button set hover state to be true
			for (auto& but : buttons) {
				if (but.click_test({ (float)e.motion.x, (float)e.motion.y }, false)) but.hover = true;
				else but.hover = false;
			}
			break;
		}
	}
}
