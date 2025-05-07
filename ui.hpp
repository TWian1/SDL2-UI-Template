#pragma once
#include <string>
#include <functional>
#include <SDL.h>
#include <SDL_ttf.h>
#include "vec.hpp"
#include <vector>
#include <iostream>

class textBox {
private:
	SDL_Renderer* renderer;
	TTF_Font* font;
	SDL_Texture* texture = nullptr;
	void updateTexture();
public:
	SDL_Rect rect;
	std::string id;
	float x, y;
	int font_size;
	bool visible = true;
	std::string text;
	std::string fontPath;
	std::vector<textBox> textBoxes;
	SDL_Color textColor, backgroundColor;
	textBox(SDL_Renderer* renderer, std::string id, TTF_Font* font, std::string fontPath, float x, float y, std::string text,
		SDL_Color textColor = { 0, 0, 0, 255 }, SDL_Color backgroundColor = { 0, 0, 0, 0 }, int font_size = -1);
	void setText(const std::string& newText);
	void Render();
};

class button {
private:
	SDL_Renderer* renderer;
	SDL_Rect rect;
	TTF_Font* font;
public:
	float x, y, w, h;
	std::string id;
	bool hover = false;
	bool visible = true;
	bool clickable = true;
	bool pressed = false;
	textBox textbox;
	SDL_Color default_color, pressed_color, hover_color;
	std::function<void()> func;
	std::vector<button> buttons;
	button(std::function<void()> func, SDL_Renderer* renderer, std::string id, std::string text, TTF_Font* font, float x, float y, float w = 100.0f, float h = 25.0f, int font_size = -1,
		SDL_Color default_color = { 255, 255, 255, 255 }, SDL_Color hover_color = { 200, 200, 200, 255 }, SDL_Color pressed_color = { 100, 100, 100, 255 }, SDL_Color text_color = { 0, 0, 0, 255 });
	bool click_test(vec2<float> MouseCoords, bool call = true);
	void Render();
};
template <typename T>
T* getObjectById(std::vector<T>& objects, const std::string& id) {
	for (auto& obj : objects) {
		if (obj.id == id) return &obj;
	}
	return nullptr;  // Return nullptr if no matching object is found
}
void renderUI(std::vector<textBox>& textBoxes, std::vector<button>& buttons);
