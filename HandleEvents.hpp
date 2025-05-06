#pragma once
#include <SDL.h>
#include <vector>
#include "vec.hpp"
#include "ui.hpp"


extern int window_width;
extern int window_height;
void handleEvents(bool& gameloop, std::vector<button>& buttons, SDL_Renderer* renderer);
