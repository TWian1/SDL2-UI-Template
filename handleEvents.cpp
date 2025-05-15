#include "HandleEvents.hpp"


void handleEvents(bool& gameloop, ui_elements& ui, SDL_Renderer* renderer) {

	// Event listener
	SDL_Event e;

	// Event loop
	while (SDL_PollEvent(&e)) {
		switch (e.type) {

		// When Quit exit game loop
		case SDL_QUIT:
			gameloop = false;
			break;

		// Key press detection
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {

			// If backspace then quit
			case SDLK_ESCAPE: 
				gameloop = false;
				break;

			// If enter then submit selected input
			case SDLK_RETURN: 
				for (auto& inp : ui.inputs) {
					if (inp.selected) {
						inp.selected = false;
						inp.Button.pressed = false;
						inp.submit_func();
						if (inp.typed.size() == 0) inp.Button.setText(inp.default_text);
					}
				}
				break;
				
			// If backspace then remove previous character on selected input
			case SDLK_BACKSPACE:
				for (auto& inp : ui.inputs) {
					if (inp.selected && inp.typed.size() > 0) {
						inp.typed.pop_back();
						inp.Button.setText(inp.typed);
					}
				}
				break;
			}
			break;

		// Text key press detection
		case SDL_TEXTINPUT:

			// Add character to selected input
			for (auto& inp : ui.inputs) {
				if (inp.selected && (inp.maxchar == 0 || inp.typed.size() < inp.maxchar)) {
					inp.typed += e.text.text;
					inp.Button.setText(inp.typed);
				}
			}
			break;

		// LMB press mechanics
		case SDL_MOUSEBUTTONDOWN:
			if (e.button.button == SDL_BUTTON_LEFT) {

				// If clicking on button call button click function
				for (auto& but : ui.buttons) {
					if (!but.visible || !but.clickable) continue;
					if (but.click_test({ (float)e.motion.x, (float)e.motion.y })) {
						but.pressed = true;
						but.func();
					}
				}

				// If clicking on input set things
				for (auto& inp : ui.inputs) {
					if (!inp.editable || !inp.visible) continue;
					if (inp.Button.click_test({ (float)e.motion.x, (float)e.motion.y })) {
						inp.selected = true;
						inp.Button.pressed = true;
						inp.Button.setText(inp.typed);

					}
					else {
						inp.selected = false;
						inp.Button.pressed = false;
						if (inp.typed.size() == 0) inp.Button.setText(inp.default_text);
					}
				}
			}
			break;

		// LMB release mechanics
		case SDL_MOUSEBUTTONUP:

			// Unset pressed state for all buttons
			if (e.button.button == SDL_BUTTON_LEFT) for (auto& but : ui.buttons) { but.pressed = false; }
			break;

		// Mouse movement mechanics
		case SDL_MOUSEMOTION:

			// If mouse hovers over button set hover state to be true
			for (auto& but : ui.buttons) {
				if (!but.visible || !but.clickable) continue;
				if (but.click_test({ (float)e.motion.x, (float)e.motion.y })) but.hover = true;
				else but.hover = false;
			}
			break;
		}
	}
}
