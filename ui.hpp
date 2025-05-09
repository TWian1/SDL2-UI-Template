#pragma once
#include <string>
#include <functional>
#include <SDL.h>
#include <SDL_ttf.h>
#include "vec.hpp"
#include <vector>
#include <iostream>

class ui_circle {
private:
	SDL_Renderer* renderer;
public:
	float x, y, r;
	bool visible = true;
	SDL_Color default_color;
	ui_circle(SDL_Renderer* renderer, float x, float y, float r, SDL_Color default_color = { 65, 65, 65, 255 });
	void Render();
};

class ui_rect {
private:
	SDL_Renderer* renderer;
	SDL_Rect rects[2];
	std::vector<ui_circle*> circles = {nullptr, nullptr, nullptr, nullptr};
public:
	float x, y, w, h, r;
	bool visible = true;
	SDL_Color default_color;
	ui_rect(SDL_Renderer* renderer, float x, float y, float w, float h, float r, SDL_Color default_color = { 65, 65, 65, 255 });
	void Render();
};

class textBox {
private:
	TTF_Font* font;
	SDL_Renderer* renderer;
	SDL_Texture* texture = nullptr;
	void updateTexture();
public:
	SDL_Rect rect;
	float x, y, r;
	int font_size;
	bool visible = true;
	std::string text, fontPath, id;
	std::vector<textBox> textBoxes;
	SDL_Color textColor, backgroundColor;
	textBox(SDL_Renderer* renderer, std::string id, TTF_Font* font, std::string fontPath, float x, float y, std::string text,
		SDL_Color textColor = { 0, 0, 0, 255 }, int font_size = -1, SDL_Color backgroundColor = { 0, 0, 0, 0 }, float r = 0);
	void setText(const std::string& newText);
	void Render();
};

class button {
private:
	SDL_Renderer* renderer;
	SDL_Rect rect;
	TTF_Font* font;
public:
	float x, y, w, h, r;
	std::string id;
	bool hover = false, visible = true, clickable = true, pressed = false;
	textBox textbox;
	SDL_Color default_color, pressed_color, hover_color;
	std::function<void()> func;
	std::vector<button> buttons;
	button(std::function<void()> func, SDL_Renderer* renderer, std::string id, std::string text, TTF_Font* font, float x, float y, float w = 100.0f, float h = 25.0f, int font_size = -1,
		SDL_Color default_color = { 255, 255, 255, 255 }, SDL_Color hover_color = { 200, 200, 200, 255 }, SDL_Color pressed_color = { 100, 100, 100, 255 }, SDL_Color text_color = { 0, 0, 0, 255 }, float r = 0);
	bool click_test(vec2<float> MouseCoords, bool call = true);
	void Render();
};

struct ui_elements {
	std::vector<button> buttons;
	std::vector<textBox> text;
	std::vector<ui_rect> rects;
	std::vector<ui_circle> circles;
};

template <typename T>
T* getObjectById(std::vector<T>& objects, const std::string& id) {
	for (auto& obj : objects) if (obj.id == id) return &obj;
	return nullptr;
}

void renderUI(ui_elements& ui);
