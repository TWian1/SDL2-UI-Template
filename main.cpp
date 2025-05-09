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
using namespace std;

// Function that is called by the button
void print(int& Generation, textBox* gentext) {
	Generation++;
	gentext->setText("Generation " + to_string(Generation));
}

//Main function
int main() {

	//Initialize SDL and SDL_TTF and check for Errors
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
		return 1;
	}
	if (TTF_Init() != 0) {
		cout << "TTF_Init Error: " << TTF_GetError() << "\n";
		SDL_Quit();
		return 1;
	}

	// Create Window and check for errors
	SDL_Window* window = SDL_CreateWindow("help", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window == nullptr) {
		cout << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
		SDL_Quit();
		return 1;
	}

	// Create renderer and check for errors
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	// Open font and check for errors
	TTF_Font* font = TTF_OpenFont("OpenSans-ExtraBold.ttf", 24);
	if (!font) {
		cout << "TTF_OpenFont Error: " << TTF_GetError() << "\n";
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

	// Initialize all of the required variables
	bool gameloop = true;
	ui_elements ui;
	Uint64 NOW = SDL_GetPerformanceCounter(), LAST = 0;
	double deltaTime = 0;

	int Generation = 0;
	ui.text.push_back(textBox(renderer, "FPS_Counter", font, "OpenSans-ExtraBold.ttf", 25, 25, "FPS: 0", {255, 255, 255, 255}, 20));
	ui.text.push_back(textBox(renderer, "Gen_Text", font, "OpenSans-ExtraBold.ttf", 25, 995, "Generation 0", { 255, 255, 255, 255 }, 50, { 0, 0, 0, 0 }, 15));
	ui.buttons.push_back(button([&]() { print(Generation, getObjectById(ui.text, "Gen_Text")); }, renderer, "Generation_Button", "TEST", font,
		1700, 970, 150.0, 75.0, 45, { 255, 66, 66, 255 }, { 255, 88, 88, 255 }, { 255, 125, 125, 255 }, { 255, 255, 255, 255 }, 12));

	// Main Game Loop
	while (gameloop) {
		//Deltatime calc
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();
		deltaTime = (double)((NOW - LAST) * 1000) / SDL_GetPerformanceFrequency(); 

		// Event Handler
		handleEvents(gameloop, ui.buttons, renderer);

		// Clears The renderer
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		//FPS Display 
		if (((NOW / 10000) % 1000 < (LAST / 10000) % 1000)) getObjectById(ui.text, "FPS_Counter")->setText("FPS: " + to_string((int)(1000.0 / deltaTime)));

		//Render UI and Display
		renderUI(ui);
		SDL_RenderPresent(renderer);
	}
	TTF_CloseFont(font);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	return 0;
}
