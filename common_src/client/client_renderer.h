#ifndef CLIENT_RENDERER_H_
#define CLIENT_RENDERER_H_

#include "../socket.h"
#include "../protocol.h"
#include <string>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/Renderer.hh>


class ClientRenderer {
    private:
    void draw_health(uint8_t n, SDL2pp::Renderer & renderer);
    void draw_rounds(uint8_t n, SDL2pp::Renderer & renderer);
    SDL2pp::Texture draw_sprite(char * path, SDL2pp::Renderer & renderer, uint8_t action);
    void render_all(uint8_t * render, SDL2pp::Renderer & renderer)


    public:

    void GameLoop();

};

#endif
