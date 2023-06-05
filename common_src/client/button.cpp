#include "button.h"
#include <iostream>

void Button::Render(SDL2pp::Renderer& renderer) {
    renderer.SetDrawColor(SDL2pp::Color(230, 230, 0, 255));
    renderer.FillRect(this->rect);
    SDL2pp::Font font("../resources/Fonts/ARIAL.TTF", 99);
    SDL2pp::Surface surface(font.RenderText_Solid(text, SDL_Color{255, 255, 255, 255}));
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