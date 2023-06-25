/*#include "mainmenu.h"
#include <iostream>

int MainMenu::Start() {
    SDL2pp::SDL sdl(SDL_INIT_VIDEO);
    SDL2pp::Window window("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_RESIZABLE); 
    SDL2pp::Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL2pp::Texture background(renderer, "../resources/backgrounds/War1/Bright/War.png");
    SDL2pp::SDLTTF sdlttf;
    int response = 0;
    while (response == 0) {
        renderer.Clear();
        response = handleEvents();
        renderer.Copy(
        background,
        SDL2pp::NullOpt,
        SDL2pp::NullOpt);
        this->boton_play.Render(renderer);
        this->boton_quit.Render(renderer);
        renderer.Present();
        SDL_Delay(1000/30);
    }
    return response;
}

int MainMenu::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (boton_quit.HandleEvent(event)) {
            return -1;
        }   
        if (boton_play.HandleEvent(event)) {
            return 1;
        }
    }
    return 0;
}

MainMenu::MainMenu() :
    boton_play("PLAY",SDL2pp::Rect((1920/2)-200,(1080/2)-10-100,400,100)),
    boton_quit("QUIT",SDL2pp::Rect((1920/2)-200,(1080/2)+10,400,100)){
}

MainMenu::~MainMenu() {
}

*/