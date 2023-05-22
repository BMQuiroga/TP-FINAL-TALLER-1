#include <iostream>
#include "client_protocol.h"
#include <string>
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

ProtocolResponse CProtocol::get(Socket &skt, bool was_closed) {
    // get the response from the server
}

void CProtocol::send(Socket &skt, ProtocolRequest request, bool was_closed) {
    // send client request
}
