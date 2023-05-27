#ifndef CLIENT_RENDERER_H_
#define CLIENT_RENDERER_H_

#include "../socket.h"
#include "../protocol.h"
#include <string>
#include <list>
#include "client_asset_manager.h"
#include "client_image.h"

class ClientRenderer {
    private:
    Queue<std::string> events;
    Queue<std::list<Image>*> updates;
    std::list<Image>* actual_frame;
    AssetManager * assets;

    SDL2pp::SDL sdl;
    SDL2pp::Window window;
    SDL2pp::Renderer renderer;

    void draw_health(uint8_t n);
    void draw_rounds(uint8_t n);
    SDL2pp::Texture draw_sprite(char * path, SDL2pp::Renderer & renderer, uint8_t action);
    void render_all();
    void render(Image im);
    void renderBackground();
    static bool handleEvents();

    public:

    void GameLoop();
    ClientRenderer(Queue<std::string> &events, Queue<std::list<Image>*> &updates);
    ~ClientRenderer();
};

#endif
