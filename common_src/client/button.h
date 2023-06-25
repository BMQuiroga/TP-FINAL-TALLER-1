#ifndef BUTTON_H_
#define BUTTON_H_

#include <string>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/Mixer.hh>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2/SDL_ttf.h>


class Button {
    //la clase tiene logica de boton en sdl
    public:
    //constructor: toma el texto y el rectangulo donde se renderiza
    Button(const std::string& text, const SDL2pp::Rect& rect);

    //dibuja el boton
    void Render(SDL2pp::Renderer& renderer, SDL2pp::Color* background_color);

    //dado un evento, devuelve true si el boton fue presionado
    bool HandleEvent(const SDL_Event& event);

    private:
    std::string text;
    SDL2pp::Rect rect;
};

#endif
