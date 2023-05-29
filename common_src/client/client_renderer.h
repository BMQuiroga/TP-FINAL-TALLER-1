#ifndef CLIENT_RENDERER_H_
#define CLIENT_RENDERER_H_

#include "../socket.h"
#include "../protocol.h"
#include <string>
#include <memory>
#include <list>
#include "intention.h"
#include "client_asset_manager.h"
#include "client_image.h"

class ClientRenderer {
    private:
    Queue<Intention*> &events;
    Queue<std::list<Image>*> &updates;
    std::list<Image>* actual_frame;
    AssetManager * assets;

    SDL2pp::SDL sdl;
    SDL2pp::Window window;
    SDL2pp::Renderer renderer;

    void draw_health(uint8_t n);
    void draw_rounds(uint8_t n);
    void render_all();
    void render(Image & im);
    void renderBackground();
    void renderHealth(uint16_t length, uint16_t x, uint16_t y, uint8_t hp);
    bool handleEvents();

    public:
    explicit ClientRenderer(Queue<Intention*> &events, Queue<std::list<Image>*> &updates);
    void GameLoop();
    ~ClientRenderer();
};

#endif
