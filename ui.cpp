#include "ui.hpp"

//Updated text and position
void textBox::updateTexture() {
	if (texture) SDL_DestroyTexture(texture);
	SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), textColor);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	rect = { (int)x, (int)y, surface->w, surface->h };
	SDL_FreeSurface(surface);
}

//Initialization function which sets new font size and updates the texture
textBox::textBox(SDL_Renderer* renderer, TTF_Font* font, std::string fontPath, float x, float y, std::string text,SDL_Color textColor, SDL_Color backgroundColor, int font_size)
	: renderer(renderer), font(font), x(x), y(y), text(text), textColor(textColor), backgroundColor(backgroundColor), font_size(font_size), fontPath(fontPath){
	if (font_size != -1) {
		TTF_Font* newFont = TTF_OpenFont(fontPath.c_str(), font_size);
		if (newFont) this->font = newFont;
	}
	updateTexture();
}

//Function for easier setting of text
void textBox::setText(const std::string& newText) {
	text = newText;
	updateTexture();
}

// Renders the finished product along with the background box to the screen
void textBox::Render() {
	if (!texture) return;
	if (!visible) return; 
	if (backgroundColor.a > 0) {
		SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
		SDL_RenderFillRect(renderer, &rect);
	}
	SDL_RenderCopy(renderer, texture, NULL, &rect);
}


//Initialization function centers textbox in button
button::button(std::function<void()> func, std::string text, TTF_Font* font, float x, float y, SDL_Renderer* renderer, float w, float h, int font_size, SDL_Color default_color, SDL_Color hover_color, SDL_Color pressed_color, SDL_Color text_color)
	: func(func), x(x), y(y), w(w), h(h), default_color(default_color), hover_color(hover_color), pressed_color(pressed_color), font(font), renderer(renderer), textbox(renderer, font, "OpenSans-ExtraBold.ttf", x, y, text, text_color, { 0, 0, 0, 0 }, font_size) {
	textbox.x = x + (w - textbox.rect.w) / 2;
	textbox.y = y + (h - textbox.rect.h) / 2;
	textbox.rect.x = (int)textbox.x;
	textbox.rect.y = (int)textbox.y;
}

// Checks to see if mouse position overlaps the bounding box of the button if so call function or return true
bool button::click_test(vec2<float> MouseCoords, bool call) {
	if (!clickable) return false;
	if (MouseCoords.x > x && MouseCoords.x < x + w && MouseCoords.y > y && MouseCoords.y < y + h) {
		if (call) func();
		return true;
	}
	return false;
}

// Renders the button to the screen
void button::Render() {
	if (!visible) return;
	rect = {(int)x, (int)y, (int)w, (int)h};
	if (!pressed) {
		if (hover) SDL_SetRenderDrawColor(renderer, hover_color.r, hover_color.g, hover_color.b, hover_color.a);
		else SDL_SetRenderDrawColor(renderer, default_color.r, default_color.g, default_color.b, default_color.a);
	}
	else SDL_SetRenderDrawColor(renderer, pressed_color.r, pressed_color.g, pressed_color.b, pressed_color.a);
	SDL_RenderFillRect(renderer, &rect);
	textbox.Render();
}