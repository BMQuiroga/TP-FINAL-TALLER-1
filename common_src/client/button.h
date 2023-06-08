#ifndef BUTTON_H_
#define BUTTON_H_

#include <string>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/Mixer.hh>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2/SDL_ttf.h>


class Button {
    public:
    Button(const std::string& text, const SDL2pp::Rect& rect);
    void Render(SDL2pp::Renderer& renderer);
    bool HandleEvent(const SDL_Event& event);

    private:
    std::string text;
    SDL2pp::Rect rect;
};

#endif
