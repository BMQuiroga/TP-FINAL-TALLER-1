#ifndef CLIENT_PROTOCOL_H_
#define CLIENT_PROTOCOL_H_

#include "../socket.h"
#include "../protocol.h"
#include "intention.h"
#include "../qt/lobby_command.h"
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

class CProtocol : public Protocol<ProtocolRequest, ProtocolResponse> {
    private:
    //manda mensajes de 1 byte
    void send_one_byte(uint8_t n, Socket &s);

    public:
    //envia una intencion (4 bytes)
    void send_command(Intention& command, Socket &s, bool *was_closed);

    //envia los comandos de lobby
    void send_lobby_command(const LobbyCommand &command, Socket &s, bool *was_closed);

    //recibe la image a renderizar del server
    uint8_t* receive_render(Socket &s);

    //arma una ProtocolResponse
    ProtocolResponse get(Socket &skt, bool *was_closed) override;
    
    //necesaria por ser virtual pura de protocol, no implementada
    void send(Socket &skt, const ProtocolRequest &request, bool was_closed) override;

    //devuelve el tipo de comando
    int get_command_type(const std::string& resource);
};

#endif
