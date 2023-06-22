#include "button.h"
#include <iostream>

void Button::Render(SDL2pp::Renderer& renderer, SDL2pp::Font& font, SDL2pp::Color* background_color) {
    //renderer.SetDrawColor(color*);
    //renderer.FillRect(this->rect);
    //SDL2pp::Font font(font, 30);
    SDL2pp::Surface surface(font.RenderText_Solid(text, background_color*));
    SDL2pp::Texture texture(renderer, surface);
    renderer.Copy(texture, SDL2pp::NullOpt, rect);
}

Button::Button(const std::string& text, const SDL2pp::Rect& rect) : text(text), rect(rect) {}

bool Button::HandleEvent(const SDL_Event& event) {
    if (event.type == SDL_EventType::SDL_MOUSEBUTTONDOWN) {
        if (rect.Contains(event.button.x, event.button.y)) {
            return true;
        }
    }
    return false;
}
