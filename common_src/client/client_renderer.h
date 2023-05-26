#ifndef CLIENT_RENDERER_H_
#define CLIENT_RENDERER_H_

#include "../socket.h"
#include "../protocol.h"
#include <string>
#include <list>
#include "client_asset_manager.h"

class ClientRenderer {
    private:
    Queue<std::string> events;
    Queue<std::list<std::string>> updates;
    AssetManager * assets;

    SDL2pp::SDL sdl;
    SDL2pp::Window window;
    SDL2pp::Renderer renderer;

    void draw_health(uint8_t n, SDL2pp::Renderer & renderer);
    void draw_rounds(uint8_t n, SDL2pp::Renderer & renderer);
    SDL2pp::Texture draw_sprite(char * path, SDL2pp::Renderer & renderer, uint8_t action);
    void render_all(uint8_t * render, SDL2pp::Renderer & renderer);
    void renderAt(Asset* asset, uint8_t x, uint8_t y, uint8_t flip);
    void renderBackground();

    public:

    void GameLoop();
    ClientRenderer(Queue<std::string> &events, Queue<std::list<std::string>> &updates);
    ~ClientRenderer();
};

#endif
