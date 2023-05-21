#ifndef CLIENT_PROTOCOL_H_
#define CLIENT_PROTOCOL_H_

#include "../socket.h"
#include "../protocol.h"
#include <string>

#define MOVE_LEFT 0
#define MOVE_RIGHT 1
#define MOVE_UP 2
#define MOVE_DOWN 3
#define STOP_MOVING_LEFT 4
#define STOP_MOVING_RIGHT 5
#define STOP_MOVING_UP 6
#define STOP_MOVING_DOWN 7

class CProtocol : public Protocol<ProtocolRequest, ProtocolResponse> {
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

    ProtocolResponse get(Socket &skt, bool was_closed) override;
    void send(Socket &skt, ProtocolRequest request, bool was_closed) override;
};

#endif
