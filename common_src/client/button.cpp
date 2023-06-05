#include "button.h"
#include <iostream>

void Button::Render(SDL2pp::Renderer& renderer) {
    renderer.DrawRect(rect, SDL2pp::Color(255, 255, 255));
    SDL2pp::Font font("../resources/Fonts/ARIAL.ttf", 28);
    SDL2pp::Surface surface(font.RenderText_Solid(text, SDL_Color{255, 255, 255, 255}));
    SDL2pp::Texture texture(renderer, surface);
    renderer.Copy(texture, SDL2pp::NullOpt, rect);
}

Button::Button(const std::string& text, const SDL2pp::Rect& rect) : text_(text), rect_(rect) {}

bool Button::HandleEvent(const SDL2pp::Event& event) {
    if (event.GetType() == SDL2pp::EventType::MOUSEBUTTONDOWN) {
        const auto& mouseEvent = static_cast<const SDL2pp::MouseButtonEvent&>(event);
        if (rect_.Contains(mouseEvent.GetX(), mouseEvent.GetY())) {
            return true;
        }
    }
    return false;
}