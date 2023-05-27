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

uint8_t * CProtocol::get(Socket &skt, bool was_closed) {
    uint8_t length;
    s.recvall(&length,1,&was_closed);
    uint8_t * render = new uint8_t[length+1];//podria ser unique ptr
    uint8_t * render_plus_one = render++;
    s.recvall(render_plus_one,length,&was_closed);
    render[0] = length;
    return render;
}

void CProtocol::send(Socket &skt, ProtocolRequest request, bool was_closed) {
    // TODO
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
    uint8_t length;
    s.recvall(&length,1);
    uint8_t * render = new uint8_t[length+1];//podria ser unique ptr
    uint8_t * render_plus_one = render++;
    s.recvall(render_plus_one,length);
    render[0] = length;
    return render;
}
