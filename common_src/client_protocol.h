#ifndef CLIENT_PROTOCOL_H_
#define CLIENT_PROTOCOL_H_

#include "socket.h"
#include <string>

class CProtocol {
    private:
    //manda mensajes de 1 byte
    void send_one_byte(uint8_t n, Socket &s);

    public:
    //procesa el evento de presionar la flecha ←
    void command_move_left(Socket &s);

    //procesa el evento de presionar la flecha →
    void command_move_right(Socket &s);

    //procesa el evento de presionar la flecha ↓
    void command_move_down(Socket &s);

    //procesa el evento de presionar la flecha ↑
    void command_move_up(Socket &s);

    //procesa el evento de dejar de presionar la flecha ←
    void command_stop_moving_left(Socket &s);

    //procesa el evento de dejar de presionar la flecha →
    void command_stop_moving_right(Socket &s);

    //procesa el evento de dejar de presionar la flecha ↓
    void command_stop_moving_down(Socket &s);

    //procesa el evento de dejar de presionar la flecha ↑
    void command_stop_moving_up(Socket &s);
};

#endif
