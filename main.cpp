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

int window_width = 1920;
int window_height = 1080;

// Function that is called by the button
void print(int& Generation, textBox& gentext) {
	Generation++;
	gentext.setText("Generation " + to_string(Generation));
	return;
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
	SDL_Window* window = SDL_CreateWindow("help", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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
	vector<button> buttons;
	vector<textBox> textBoxes;
	srand(time(NULL));
	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;

	int Generation = 0;
	textBox fps_counter(renderer, font, "OpenSans-ExtraBold.ttf", 5, 5, "FPS: 0", { 255, 255, 255, 255 }, { 0, 0, 0, 0 }, 20);
	textBoxes.push_back(fps_counter);

	textBox tex1(renderer, font, "OpenSans-ExtraBold.ttf", 15, 1005, "Generation 0", { 255, 255, 255, 255 }, { 0, 0, 0, 0 }, 50);
	textBoxes.push_back(tex1);

	button but1([&]() { print(Generation, textBoxes[1]); }, "TEST", font, 1700, 970, renderer, 150.0, 75.0, 45, { 255, 66, 66, 255 }, { 255, 88, 88, 255 }, { 255, 125, 125, 255 }, { 255, 255, 255, 255 });
	buttons.push_back(but1);

	// Main Game Loop
	while (gameloop) {
		//FPS calc
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();
		deltaTime = (double)((NOW - LAST) * 1000) / SDL_GetPerformanceFrequency(); 

		// Event Handler
		handleEvents(gameloop, buttons, renderer);

		// Clears The renderer
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Render buttons and text
		for (auto& but : buttons) {but.Render();}
		for (auto& tex : textBoxes) {tex.Render();}

		//FPS Display 
		if (((NOW / 10000) % 1000 < (LAST / 10000) % 1000)) textBoxes[0].setText("FPS: " + to_string((int)(1000.0 / deltaTime)));

		//Display
		SDL_RenderPresent(renderer);
	}

	// Quit the window
	TTF_CloseFont(font);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	return 0;
}
