#ifndef CLIENT_PROTOCOL_H_
#define CLIENT_PROTOCOL_H_

#include "../socket.h"
#include "../protocol.h"
#include "intention.h"
#include <string>

#define MOVE_LEFT 0
#define MOVE_RIGHT 1
#define MOVE_UP 2
#define MOVE_DOWN 3
#define STOP_MOVING_LEFT 4
#define STOP_MOVING_RIGHT 5
#define STOP_MOVING_UP 6
#define STOP_MOVING_DOWN 7
#define SHOOT 8
#define STOP_SHOOTING 9

class CProtocol : public Protocol<ProtocolRequest, uint8_t*> {
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

    void command_shoot(Socket &s);

    void command_stop_shooting(Socket &s);
    
    void send_command(Intention& command, Socket &s, bool *was_closed);

    //recibe la image a renderizar del server
    uint8_t* receive_render(Socket &s);

    ProtocolResponse get(Socket &skt, bool *was_closed);
    
    // void send(Socket &skt, ProtocolRequest request, bool was_closed) override;
};

#endif
