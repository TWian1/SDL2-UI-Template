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
void print(int& Generation, textBox& gentext) {
	Generation++;
	gentext.setText("Generation " + to_string(Generation));
	return;
}

//Main function
int main() {
	// Set the window scrren height and width to that of the display bounds
	constexpr int screen_width = 1920;
	constexpr int screen_height = 1080;
	constexpr bool fpsCounterEnable = true;

	//Initialize SDL and SDL_TTF and check for Errors
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
		return 1;
	}
	if (TTF_Init() != 0) {
		std::cerr << "TTF_Init Error: " << TTF_GetError() << "\n";
		SDL_Quit();
		return 1;
	}

	// Create Window and check for errors
	SDL_Window* window = SDL_CreateWindow("help", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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
		std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << "\n";
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

	// Initialize all of the required variables
	int frameCount = 0;
	bool gameloop = true;
	unsigned long lastFPScheck = 0;
	long long currentFrameTime = SDL_GetTicks();
	long long frameTime;
	SDL_Texture* textTexture = nullptr;
	SDL_Rect textRect;
	vector<button> buttons;
	vector<textBox> textBoxes;
	srand(time(NULL));
	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;

	//Update topleft text function
	auto updateText = [&](const std::string& newText) {
		if (textTexture) SDL_DestroyTexture(textTexture);
		SDL_Surface* textSurface = TTF_RenderText_Blended(font, newText.c_str(), { 255, 255, 255, 255 });
		textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		textRect = { 5, 5, textSurface->w, textSurface->h };
		SDL_FreeSurface(textSurface); };
	updateText("FPS: 0");

	// Main Game Loop
	int Generation = 0;

	textBox tex1(renderer, font, "OpenSans-ExtraBold.ttf", 15, 1005, "Generation 0", { 255, 255, 255, 255 }, { 0, 0, 0, 0 }, 50);
	textBoxes.push_back(tex1);

	textBox tex2(renderer, font, "OpenSans-ExtraBold.ttf", 150, 150, "AAAA", { 255, 255, 255, 255 }, { 0, 0, 0, 0 }, 50);
	textBoxes.push_back(tex2);

	button but1([&]() { print(Generation, textBoxes[0]); }, "TEST", font, 1700, 970, renderer, 150.0, 75.0, 45, { 255, 66, 66, 255 }, { 255, 88, 88, 255 }, { 255, 125, 125, 255 }, { 255, 255, 255, 255 });
	buttons.push_back(but1);


	while (gameloop) {
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();
		deltaTime = (double)((NOW - LAST) * 1000) / SDL_GetPerformanceFrequency(); // in ms

		// Event Handler
		handleEvents(gameloop, buttons);

		// Clears The renderer and increments the frame counter
		frameCount++;

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);


		// Render buttons and text
		for (auto& but : buttons) {
			but.Render();
		}
		for (auto& tex : textBoxes) {
			tex.Render();
		}


		//FPS Display Logic
		frameTime = SDL_GetTicks();
		if (fpsCounterEnable) {
			if (frameTime - lastFPScheck > 500) {
				updateText("FPS: " + std::to_string((int)((frameCount * 1000) / (frameTime - lastFPScheck))));
				lastFPScheck = frameTime;
				frameCount = 0;
			}
			SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
		}

		//Display
		SDL_RenderPresent(renderer);
	}

	// Quit the window
	if (textTexture) SDL_DestroyTexture(textTexture);
	TTF_CloseFont(font);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	return 0;
}