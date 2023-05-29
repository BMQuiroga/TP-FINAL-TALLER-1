#ifndef SERVER_PROTOCOL_H_
#define SERVER_PROTOCOL_H_

#include "../socket.h"
#include "../protocol.h"
#include <string>

class ServerProtocol : public Protocol<ProtocolResponse, ProtocolRequest> {
    public:
    //recibe un puntero a char que contiene ya codificado
    //el mensaje de la imagen y lo envia
    void send_render(char * data, int length, Socket & s);

    //recibe un byte
    uint8_t receive_command(Socket & s);

    ProtocolRequest get(Socket &skt, bool *was_closed) override;
    void send(Socket &skt,ProtocolResponse resp, bool was_closed) override;
};

#endif
