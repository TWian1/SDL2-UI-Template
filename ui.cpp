#include "ui.hpp"

// Initialization function for circle
ui_circle::ui_circle(SDL_Renderer* renderer, std::string id, float x, float y, float r, SDL_Color default_color,int z) : renderer(renderer), id(id), x(x), y(y), r(r), z(z), default_color(default_color) {};

// Renders circle to screen
void ui_circle::Render() {
	if (!visible || r == 0) return;
	SDL_SetRenderDrawColor(renderer, default_color.r, default_color.g, default_color.b, default_color.a);
	for (int w = 0; w < r * 2; w++) for (int h = 0; h < r * 2; h++) if (((r - w) * (r - w) + (r - h) * (r - h)) <= (r * r)) SDL_RenderDrawPoint(renderer, static_cast<int>(x + (r - w)), static_cast<int>(y + (r - h)));
}

// Initialization function for ui_rect 
ui_rect::ui_rect(SDL_Renderer* renderer, std::string id, float x, float y, float w, float h, float r, SDL_Color default_color,int z) : renderer(renderer), id(id), x(x), y(y), w(w), h(h), r(r), z(z), default_color(default_color) {
	rects[0] = { static_cast<int>(x), static_cast<int>(y + r), static_cast<int>(w), static_cast<int>(h - (2 * r)) };
	rects[1] = { static_cast<int>(x + r), static_cast<int>(y), static_cast<int>(w - (2 * r)), static_cast<int>(h) };
	for (int c = 0; c < 4; c++) circles[c] = new ui_circle(renderer, id+"_c"+std::to_string(c), (x + r + ((static_cast<int>(c / 2)) * (w - (2 * r)))), (y + r + ((c % 2) * (h - (2 * r)))), r, default_color, z);
}

// Renders UI rect to the screen(2 rects and 4 circles)
void ui_rect::Render() {
	if (!visible) return;
	SDL_SetRenderDrawColor(renderer, default_color.r, default_color.g, default_color.b, default_color.a);
	SDL_RenderFillRects(renderer, rects, 2);
	for (auto c : circles) c->Render();
}

// Update texture so that text is aligned and matches the text variable
void textBox::updateTexture() {
	if (texture) SDL_DestroyTexture(texture);
	if (text.size() == 0) return;
	SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), textColor);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	rect = { static_cast<int>(x), static_cast<int>(y), surface->w, surface->h };
	SDL_FreeSurface(surface);
}

// Initialization function for Text Box
textBox::textBox(SDL_Renderer* renderer, std::string id, TTF_Font* font, std::string fontPath, float x, float y, std::string text, SDL_Color textColor, int font_size, int z)
	: renderer(renderer), font(font), x(x), y(y), z(z), text(text), textColor(textColor), font_size(font_size), fontPath(fontPath), id(id) {
	if (font_size != -1) {this->font = TTF_OpenFont(fontPath.c_str(), font_size);}
	updateTexture();
}

// Function that sets text and renders it
void textBox::setText(const std::string& newText) {
	text = newText;
	updateTexture();
}

// Renders the finished textBox product to the screen
void textBox::Render() {
	if (!texture || !visible) return;
	SDL_RenderCopy(renderer, texture, NULL, &rect);
}

// Function that sets the text and renders it for a button object
void button::setText(const std::string& newText) {
	textbox.setText(newText);
	updateText();
}

// Sets the button text so its aligned correctly
void button::updateText() {
	if (align_c) textbox.x = x + (w - textbox.rect.w) / 2;
	else textbox.x = x + 7;
	textbox.rect.x = static_cast<int>(textbox.x);
	textbox.y = y + (h - textbox.rect.h) / 2;
	textbox.rect.y = static_cast<int>(textbox.y);
}

// Initialization function for button
button::button(std::function<void()> func,SDL_Renderer* renderer,std::string id,std::string text,TTF_Font* font,std::string font_p,float x,float y,float w,float h,int font_size,SDL_Color default_c,SDL_Color hover_c,SDL_Color press_c,SDL_Color text_c,float r,bool align_center,int z): 
func(func),x(x),y(y),w(w),h(h),default_c(default_c),hover_c(hover_c),press_c(press_c),id(id),font(font),renderer(renderer),z(z),align_c(align_center),textbox(renderer,id+"_Text",font,font_p,x,y,text,text_c,font_size, z),r(r){updateText();}

// Checks to see if mouse position overlaps the bounding box of the button if so call function or return true
bool button::click_test(vec2<float> MouseCoords) {
	if (!clickable) return false;
	if (MouseCoords.x > x && MouseCoords.x < x + w && MouseCoords.y > y && MouseCoords.y < y + h) {return true;}
	return false;
}

// Renders the button to the screen
void button::Render() {
	if (!visible) return;
	SDL_Color outcolor;
	if (!pressed) {
		if (hover) outcolor = hover_c;
		else outcolor = default_c;
	}
	else outcolor = press_c;
	if (outcolor.a > 0) {
		ui_rect newrect(renderer, id+"_rect", x, y, w, h, r, outcolor, z);
		newrect.Render();
	}
	textbox.Render();
}

// Initialization Function for text input
textInput::textInput(std::function<void()> submit_func, SDL_Renderer* renderer,std::string id,std::string default_text,TTF_Font* font, std::string font_path, float x, float y, float w, float h, int font_size,SDL_Color default_color, SDL_Color selected_color, SDL_Color text_color, float r, int maxchar, bool align_center, int z) : 
submit_func(submit_func), maxchar(maxchar), id(id),default_text(default_text), z(z), Button([&]() {}, renderer,id + "_Button", default_text, font, font_path, x, y, w, h, font_size, default_color, default_color, selected_color, text_color, r, align_center, z) {}

// Renders the textInput
void textInput::Render() { 
	if (!visible) return;
	Button.Render(); 
}

// Renders all UI in ui struct
void renderUI(ui_elements& ui) {
	for (
		int z_render = -10; // Z minimum
		z_render <= 10; // Z maxiumum
		z_render++) {
		for (auto& cir : ui.circles) { if (cir.z == z_render) cir.Render(); }
		for (auto& box : ui.rects) { if (box.z == z_render) box.Render(); }
		for (auto& but : ui.buttons) { if (but.z == z_render) but.Render(); }
		for (auto& tex : ui.text) { if (tex.z == z_render) tex.Render(); }
		for (auto& inp : ui.inputs) { if (inp.z == z_render) inp.Render(); }
	}
}
