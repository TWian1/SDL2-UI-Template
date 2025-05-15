#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <functional>
#include "vec.hpp"
#include "HandleEvents.hpp"
#include "ui.hpp"
#undef main // SDL breaks without this for some reason

// Function that is called by the button
void print(int& Generation, textBox* gentext) {
	Generation++;
	gentext->setText("Generation " + std::to_string(Generation));
}

//Main function
int main() {
	// Initialize SDL and SDL_TTF and check for Errors
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl; return 1; }
	if (TTF_Init() != 0) { std::cout << "TTF_Init Error: " << TTF_GetError() << std::endl; SDL_Quit(); return 1; }
	SDL_Window* window = SDL_CreateWindow("help", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) { std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl; SDL_DestroyWindow(window); SDL_Quit(); return 1; }

	//Font definition
	const std::string font_path_1 = "OpenSans-ExtraBold.ttf", font_path_2 = "OpenSans.ttf";

	// Set Fonts
	TTF_Font* font = TTF_OpenFont(font_path_1.c_str(), 24);
	TTF_Font* font2 = TTF_OpenFont(font_path_2.c_str(), 24);
	if (!font2 || !font) { std::cout << "TTF_OpenFont Error: " << TTF_GetError() << std::endl; SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window); TTF_Quit(); SDL_Quit(); return 1; }

	// Initialize all of the required variables
	bool gameloop = true;
	ui_elements ui;
	Uint64 NOW = SDL_GetPerformanceCounter(), LAST = 0;
	double deltaTime = 0;

	// UI Code
	int Generation = 0;
	ui.text.push_back(textBox(renderer, "FPS_Counter", font, font_path_1, 25, 25, "FPS: 0", { 255, 255, 255, 255 }, 20));
	ui.text.push_back(textBox(renderer, "Gen_Text", font, font_path_1, 25, 995, "Generation 0", { 255, 255, 255, 255 }, 50));
	ui.buttons.push_back(button([&]() { print(Generation, getObjectById(ui.text, "Gen_Text")); }, renderer, "Generation_Button", "TEST", font, font_path_1,
		1700, 970, 150.0, 75.0, 45, { 255, 66, 66, 255 }, { 255, 88, 88, 255 }, { 255, 125, 125, 255 }, { 255, 255, 255, 255 }, 12));
	ui.text.push_back(textBox(renderer, "outtext", font, font_path_1, 250, 250, " ", { 255,255,255,255 }));
	ui.inputs.push_back(textInput([&]() {getObjectById(ui.text, "outtext")->setText(getObjectById(ui.inputs, "inputtest")->typed); }, renderer, "inputtest", "test", font2, font_path_2, 100, 100, 200, 40, 20, { 100, 100, 100, 100 }, { 60, 60, 60, 100 }, { 255, 255, 255, 255 }, 10.0, 7, false));

	// Main Game Loop
	while (gameloop) {
		// Deltatime calc
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();
		deltaTime = (double)((NOW - LAST) * 1000) / SDL_GetPerformanceFrequency();

		// Event Handler
		handleEvents(gameloop, ui, renderer);

		// Clears The renderer
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// FPS Display 
		if (((NOW / 10000) % 1000 < (LAST / 10000) % 1000)) getObjectById(ui.text, "FPS_Counter")->setText("FPS: " + std::to_string((int)(1000.0 / deltaTime)));

		// Render UI and Display
		renderUI(ui);
		SDL_RenderPresent(renderer);
	}


	// Quit SDL and all things running
	TTF_CloseFont(font);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	return 0;
}
