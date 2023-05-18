#include <iostream>
#include "client_protocol.h"
#include <string>
//#include <arpa/inet.h>

void CProtocol::send_one_byte(uint8_t n, Socket &s) {
    s.sendall(&n,1);
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