#include <iostream>
#include "client_renderer.h"
#include <string>
//#include <arpa/inet.h>

ClientRenderer::ClientRenderer(Queue<std::string> &events, Queue<std::list<std::string>> &updates) : 
    events(events),
    updates(updates),
    sdl(SDL_INIT_VIDEO),
    window("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_RESIZABLE),
    renderer(window, -1, SDL_RENDERER_ACCELERATED) {
    this->assets = AssetManager::Instance(this->renderer);
}

void ClientRenderer::draw_health(uint8_t n, SDL2pp::Renderer & renderer) {

}

void ClientRenderer::draw_rounds(uint8_t n, SDL2pp::Renderer & renderer) {
    
}

SDL2pp::Texture ClientRenderer::draw_sprite(char * path, SDL2pp::Renderer & renderer, uint8_t action) {
    SDL2pp::Texture sprite(
        renderer,
        SDL2pp::Surface(path).SetColorKey(true,0x000000)
    );
    return sprite;
}

void ClientRenderer::renderAt(Asset* asset, uint8_t x, uint8_t y, uint8_t flip) {

}

void ClientRenderer::renderBackground() {
    Asset * asset = assets->GetAsset(0);
    renderer.Copy(
        (*asset->get_texture()),
        SDL2pp::NullOpt,
        SDL2pp::Rect(0, 0, 1920, 1080),
        0.0,                // don't rotate
        SDL2pp::NullOpt,    // rotation center - not needed
        SDL_FLIP_NONE
    );
}

void ClientRenderer::GameLoop() {
    uint32_t frames = 0
    while (1) {
        renderer.Clear();
        renderBackground();
        if (updates.try_pop()) {

        }




        renderer.Present();
    }
}


void ClientRenderer::render_all(uint8_t * render, SDL2pp::Renderer & renderer) {
    uint16_t * iterator16 = nullptr;
    uint16_t x, y;
    uint8_t flip, action;
    renderer.Clear();
    uint8_t * iterator = render;
    draw_health((*iterator++),renderer);
    draw_rounds((*iterator++),renderer);
    while(iterator) {
        switch (*iterator++) {
            case 1: {
                iterator16 = reinterpret_cast<uint16_t*>(iterator);
                x = *iterator16++;
                y = *iterator16;
                iterator+=4;
                flip = *iterator++;
                action = *iterator++;//unused
                SDL2pp::Texture sprite = draw_sprite("../resources/Soldier_1/Idle.png",renderer,action);
                renderer.Copy(
                    sprite,
                    SDL2pp::Rect(0, 0, 130, 130),
                    SDL2pp::Rect(x, y, x + 130 - 1, y + 130 - 1),
                    0,
                    SDL2pp::Point(),
                    flip > 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE
                );
                iterator+=4;
                break;
            }
            case 2: {
                iterator16 = reinterpret_cast<uint16_t*>(iterator);
                x = *iterator16++;
                y = *iterator16;
                iterator+=4;
                flip = *iterator++;
                action = *iterator++;//unused
                SDL2pp::Texture sprite = draw_sprite("../resources/Soldier_2/Idle.png",renderer,action);
                renderer.Copy(
                    sprite,
                    SDL2pp::Rect(0,0,130,130),
                    SDL2pp::Rect(x,y,x+130,y+130),
                    0,
                    SDL2pp::Point(),
                    flip > 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE
                );
                iterator+=4;
                break;
            }
            default:
                break;
        }
    }
    renderer.Present();
}

ClientRenderer::~ClientRenderer() {
    AssetManager::Release();
}