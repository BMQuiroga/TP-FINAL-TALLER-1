#include <iostream>
#include "client_renderer.h"
#include <string>
#include "intention.h"
#include <cmath>
//#include <arpa/inet.h>

void ClientRenderer::GameLoop() {
    std::list<Image>* new_update = nullptr;
    bool running = true;
    while (running) {
        unsigned int frame_ticks = SDL_GetTicks();	
        running = this->handleEvents();
        renderer.Clear();    
        if (updates.try_pop(new_update)) {
            actual_frame = Image::Replace(actual_frame,new_update);
        }
        renderBackground();
        render_all();
        renderer.Present();
        unsigned int end_ticks = SDL_GetTicks();
        unsigned int ticks_delta = frame_ticks - end_ticks;
        SDL_Delay((1000/30) - ticks_delta);
    }
}

void ClientRenderer::render_all() {
    if (actual_frame != nullptr) {
        draw_health(actual_frame->front().id);
        draw_rounds(actual_frame->front().action);
        for (auto const& it : *actual_frame) {
            if (it.id > 0) {
                render(const_cast<Image&>(it));
            }
        }
    }
}

void ClientRenderer::render(Image & im) {
    uint16_t x = im.x;
    uint16_t y = im.y;
    Asset * asset = assets->GetAsset(im.id + im.action*1000);

    while (im.frame > asset->get_frames())
        im.frame -=  asset->get_frames();
    
    renderer.Copy(
        (*asset->get_texture()),
        SDL2pp::Rect(asset->get_length() * im.frame, 0, (asset->get_length() * im.frame) + asset->get_length(), asset->get_height()),
        SDL2pp::Rect(x, y, x + asset->get_length() - 1, y + asset->get_height() - 1),
        0,
        SDL2pp::NullOpt,
        im.flip > 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE
    );
    if (im.health != 0)
        renderHealth(asset->get_length(),x,y,im.health);
}

void ClientRenderer::renderHealth(uint16_t length, uint16_t x, uint16_t y, uint8_t hp) {
    uint16_t hp_bar_height = 3;
    uint16_t hp_bar_height_difference = 10;
    Asset * full = assets->GetAsset(-1);
    Asset * empty = assets->GetAsset(-2);
    float hp_percentage = (x + length -1)*hp/100;

    renderer.Copy(
        (*empty->get_texture()),
        SDL2pp::Rect(0,0,length,hp_bar_height),
        SDL2pp::Rect(x, y + hp_bar_height_difference, x + length - 1, y + hp_bar_height - 1)

    );
    renderer.Copy(
        (*full->get_texture()),
        SDL2pp::Rect(0,0,length,hp_bar_height),
        SDL2pp::Rect(x, y + hp_bar_height_difference, std::round(hp_percentage), y + hp_bar_height - 1)
    );
}


ClientRenderer::ClientRenderer(Queue<Intention*> &events, Queue<std::list<Image>*> &updates) : 
    events(events),
    updates(updates),
    sdl(SDL_INIT_VIDEO),
    window("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_RESIZABLE),
    actual_frame(nullptr),
    renderer(window, -1, SDL_RENDERER_ACCELERATED) {
    this->assets = AssetManager::Instance(this->renderer);
}

void ClientRenderer::draw_health(uint8_t n) {
    //se va, cada unidad va a tener su vida
}

void ClientRenderer::draw_rounds(uint8_t n) {
    
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

ClientRenderer::~ClientRenderer() {
    AssetManager::Release();
}

bool ClientRenderer::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        events.push(Intention::make_intention(event));
    }
    return true;
}
