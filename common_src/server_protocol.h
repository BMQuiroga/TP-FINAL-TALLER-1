#ifndef SERVER_PROTOCOL_H_
#define SERVER_PROTOCOL_H_

#include "socket.h"
#include <string>

class SProtocol {
    public:
    //recibe un puntero a char que contiene ya codificado
    //el mensaje de la imagen y lo envia
    void send_render(char * data, int lenght, Socket & s);

    //recibe un byte
    uint8_t receive_command(Socket & s);
};

#endif
