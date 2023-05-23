#include <iostream>
#include "client_protocol.h"
#include <string>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/Renderer.hh>
//#include <arpa/inet.h>

void CProtocol::send_one_byte(uint8_t n, Socket &s) {
    s.sendall(&n,ONE_BYTE);
}

void CProtocol::command_move_left(Socket &s) {
    send_one_byte(MOVE_LEFT,s);
}

void CProtocol::command_move_right(Socket &s) {
    send_one_byte(MOVE_RIGHT,s);
}

void CProtocol::command_move_down(Socket &s) {
    send_one_byte(MOVE_DOWN,s);
}

void CProtocol::command_move_up(Socket &s) {
    send_one_byte(MOVE_UP,s);
}

void CProtocol::command_stop_moving_left(Socket &s) {
    send_one_byte(STOP_MOVING_LEFT,s);
}

void CProtocol::command_stop_moving_right(Socket &s) {
    send_one_byte(STOP_MOVING_RIGHT,s);
}

void CProtocol::command_stop_moving_down(Socket &s) {
    send_one_byte(STOP_MOVING_DOWN,s);
}

void CProtocol::command_stop_moving_up(Socket &s) {
    send_one_byte(STOP_MOVING_UP,s);
}

void CProtocol::command_shoot(Socket &s) {
    send_one_byte(SHOOT,s);
}

void CProtocol::command_stop_shooting(Socket &s) {
    send_one_byte(STOP_SHOOTING,s);
}

ProtocolResponse CProtocol::get(Socket &skt, bool was_closed) {
    // TODO: recibir respuesta del servidor y pushearla a una queue
}

void CProtocol::send_command(const std::string& command, Socket &s) {
    if (command == "move left") {
        command_move_left(s);
    } else if (command == "move right") {
        command_move_right(s);
    } else if (command == "move up") {
        command_move_up(s);
    } else if (command == "move down") {
        command_move_down(s);
    } else if (command == "stop left") {
        command_stop_moving_left(s);
    } else if (command == "stop right") {
        command_stop_moving_right(s);
    } else if (command == "stop up") {
        command_stop_moving_up(s);
    } else if (command == "stop down") {
        command_stop_moving_down(s);
    } else if (command == "shoot") {
        command_shoot(s);
    } else if (command == "stop shooting") {
        command_stop_shooting(s);
    }
}

uint8_t* CProtocol::receive_render(Socket &s) {
    uint8_t lenght;
    s.recvall(&lenght,1);
    uint8_t * render = new uint8_t[lenght];//podria ser unique ptr
    s.recvall(render,lenght);
    return render;
}

void draw_health(uint8_t n, SDL2pp::Renderer & renderer) {

}

void draw_rounds(uint8_t n, SDL2pp::Renderer & renderer) {
    
}

SDL2pp::Texture draw_sprite(char * path, SDL2pp::Renderer & renderer, uint8_t action) {
    SDL2pp::Texture sprite(
        renderer,
        SDL2pp::Surface(path).SetColorKey(true,0x000000)
    );
    return sprite;
}


void render_all(uint8_t * render, SDL2pp::Renderer & renderer) {
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