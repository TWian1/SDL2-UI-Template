#include "ui.hpp"

// initialization function for circle
ui_circle::ui_circle(SDL_Renderer* renderer, float x, float y, float r, SDL_Color default_color) : renderer(renderer), x(x), y(y), r(r), default_color(default_color) {};

// Renders circle to screen
void ui_circle::Render() {
	if (!visible || r == 0) return;
	SDL_SetRenderDrawColor(renderer, default_color.r, default_color.g, default_color.b, default_color.a);
	for (int w = 0; w < r * 2; w++) for (int h = 0; h < r * 2; h++) if (((r - w)*(r - w) + (r - h) * (r - h)) <= (r * r)) SDL_RenderDrawPoint(renderer, static_cast<int>(x + (r - w)), static_cast<int>(y + (r - h)));
}

// Initialization function for ui_rect creates rects and circles
ui_rect::ui_rect(SDL_Renderer* renderer, float x, float y, float w, float h, float r, SDL_Color default_color) : renderer(renderer), x(x), y(y), w(w), h(h), r(r), default_color(default_color) {
	rects[0] = { static_cast<int>(x), static_cast<int>(y + r), static_cast<int>(w), static_cast<int>(h - (2 * r))};
	rects[1] = { static_cast<int>(x + r), static_cast<int>(y), static_cast<int>(w - (2 * r)), static_cast<int>(h)};
	for (int c = 0; c < 4; c++) circles[c] = new ui_circle(renderer, (x + r + ((static_cast<int>(c/2))*(w-(2*r)))), (y + r +((c%2)*(h-(2*r)))), r, default_color);
}

//Renders UI rect to the screen(2 rects and 4 circles)
void ui_rect::Render() {
	if (!visible) return;
	SDL_SetRenderDrawColor(renderer, default_color.r, default_color.g, default_color.b, default_color.a);
	SDL_RenderFillRects(renderer, rects, 2);
	for (auto c : circles) c->Render();
}

//Updated text and position
void textBox::updateTexture() {
	if (texture) SDL_DestroyTexture(texture);
	SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), textColor);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	rect = { static_cast<int>(x), static_cast<int>(y), surface->w, surface->h };
	SDL_FreeSurface(surface);
}

//Initialization function which sets new font size and updates the texture
textBox::textBox(SDL_Renderer* renderer, std::string id, TTF_Font* font, std::string fontPath, float x, float y, std::string text,SDL_Color textColor, int font_size, SDL_Color backgroundColor,  float r)
	: renderer(renderer), font(font), x(x), y(y), text(text), textColor(textColor), backgroundColor(backgroundColor), font_size(font_size), fontPath(fontPath), id(id), r(r){
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
	if (!texture || !visible) return;
	if (backgroundColor.a > 0) {
		ui_rect newrect(renderer, x, y, static_cast<float>(rect.w), static_cast<float>(rect.h), r, backgroundColor);
		newrect.Render();
	}
	SDL_RenderCopy(renderer, texture, NULL, &rect);
}

//Initialization function centers textbox in button
button::button(std::function<void()> func, SDL_Renderer* renderer, std::string id, std::string text, TTF_Font* font, float x, float y, float w, float h, int font_size, SDL_Color default_color, SDL_Color hover_color, SDL_Color pressed_color, SDL_Color text_color, float r)
	: func(func), x(x), y(y), w(w), h(h), default_color(default_color), hover_color(hover_color), pressed_color(pressed_color), id(id), font(font), renderer(renderer), textbox(renderer, id+"_Text", font, "OpenSans-ExtraBold.ttf", x, y, text, text_color, font_size), r(r) {
	textbox.x = x + (w - textbox.rect.w) / 2;
	textbox.y = y + (h - textbox.rect.h) / 2;
	textbox.rect.x = static_cast<int>(textbox.x);
	textbox.rect.y = static_cast<int>(textbox.y);
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
	SDL_Color outcolor;
	rect = { static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h)};
	if (!pressed) {
		if (hover) outcolor = hover_color;
		else outcolor = default_color;
	}
	else outcolor = pressed_color;
	if (outcolor.a > 0) {
		ui_rect newrect(renderer, x, y, w, h, r, outcolor);
		newrect.Render();
	}
	textbox.Render();
}

// Renders all UI in vectors
void renderUI(ui_elements& ui){
	for (auto& cir : ui.circles) { cir.Render(); }
	for (auto& box : ui.rects) { box.Render(); }
	for (auto& but : ui.buttons) { but.Render(); }
	for (auto& tex : ui.text) { tex.Render(); }
}
