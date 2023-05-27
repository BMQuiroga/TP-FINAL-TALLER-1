#include <iostream>
#include "client_renderer.h"
#include <string>
//#include <arpa/inet.h>

void ClientRenderer::GameLoop() {
    uint32_t frames = 0;
    std::list<Image>* new_update = nullptr;
    bool running = true;
    while (running) {
        running = handle_events();
        renderer.Clear();    
        if (updates.try_pop(new_update)) {
            actual_frame = Image::Replace(actual_frame,new_update);
        }
        renderBackground();
        render_all();
        renderer.Present();
        //falta sdl sleep;
    }
}

void ClientRenderer::render_all() {
    draw_health(actual_frame->front().id);
    draw_rounds(actual_frame->front().action);
    for (auto const& it : actual_frame) {
        if(it->id > 0) {
            render((*it));
        }
    }
}

void ClientRenderer::render(Image & im) {
    uint16_t x = im.x;
    uint16_t y = im.y;
    Asset * asset = assets->GetAsset(im.id + im.action*1000);
    renderer.Copy(
                    asset,
                    SDL2pp::Rect(asset->get_length() * im.frame, 0, (asset->get_length() * im.frame) + asset->get_length(), asset->get_height()),
                    SDL2pp::Rect(x, y, x + asset->get_length() - 1, y + asset->get_height() - 1),
                    0,
                    SDL2pp::NullOpt,
                    im.flip > 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE
    );
    //dibujarle la barra de vida arriba
}


ClientRenderer::ClientRenderer(Queue<std::string> &events, Queue<std::list<Image>*> &updates) : 
    events(events),
    updates(updates),
    sdl(SDL_INIT_VIDEO),
    window("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_RESIZABLE),
    renderer(window, -1, SDL_RENDERER_ACCELERATED) {
    this->assets = AssetManager::Instance(this->renderer);
}

void ClientRenderer::draw_health(uint8_t n) {
    //se va, cada unidad va a tener su vida
}

void ClientRenderer::draw_rounds(uint8_t n) {
    
}

SDL2pp::Texture ClientRenderer::draw_sprite(char * path, SDL2pp::Renderer & renderer, uint8_t action) {
    SDL2pp::Texture sprite(
        renderer,
        SDL2pp::Surface(path).SetColorKey(true,0x000000)
    );
    return sprite;
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


/*
void ClientRenderer::render_all() {
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
}*/

ClientRenderer::~ClientRenderer() {
    AssetManager::Release();
}

static bool ClientRenderer::handleEvents() {
    SDL_Event event;
    // Para el alumno: Buscar diferencia entre waitEvent y pollEvent
    while(SDL_PollEvent(&event)){
        switch(event.type) {
            case SDL_KEYDOWN: {
                // ¿Qué pasa si mantengo presionada la tecla?    
                SDL_KeyboardEvent& keyEvent = (SDL_KeyboardEvent&) event;
                switch (keyEvent.keysym.sym) {
                    case SDLK_LEFT:
                        //player.moveLeft();
                        break;
                    case SDLK_RIGHT:
                        //player.moveRigth();
                        break;
                    case SDLK_DOWN:
                        //player.moveRigth();
                        break;
                    case SDLK_UP:
                        //player.moveRigth();
                        break;
                    }
                } // Fin KEY_DOWN
                break;
            case SDL_KEYUP: {
                SDL_KeyboardEvent& keyEvent = (SDL_KeyboardEvent&) event;
                switch (keyEvent.keysym.sym) {
                    case SDLK_LEFT:
                        //player.stopMoving();
                        break;
                    case SDLK_RIGHT:
                        //player.stopMoving();
                        break;
                    case SDLK_DOWN:
                        //player.moveRigth();
                        break;
                    case SDLK_UP:
                        //player.moveRigth();
                        break;
                    } 
                }// Fin KEY_UP
                break;
            case SDL_MOUSEMOTION:
                //std::cout << "Oh! Mouse" << std::endl;
                break;
            case SDL_QUIT:
                //std::cout << "Quit :(" << std::endl;
                return false;
        } // fin switch(event)
    } // fin while(SDL_PollEvents)
    return true;
}