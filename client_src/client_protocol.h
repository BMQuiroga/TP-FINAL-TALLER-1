#ifndef CLIENTPROTOCOL_H
#define CLIENTPROTOCOL_H
#include <stdint.h>
#include <iostream>
#include "../common_src/socket.h"
#include "../server_src/matchstate.h"
#include "../common_src/protocol.h"
#include <string>
#include <vector>
#include <sstream>
#include <tuple>

class ClientProtocol:public Protocol {
    public:
    ClientProtocol() = default;
    /**
     * Recibe el comando y los parametros necesarios 
     * para que se ejecute como argumentos y se los
     * envía al servidor.
    */
    void send_command(
        const std::string& data, 
        const std::string& parameter, 
        Socket &skt);
    /**
     * Recibe el comando y los parametros necesarios 
     * para que se ejecute como argumentos y,
     * según el comando recibido, serializa los datos
     * de la forma pedida
    */
    void send_info_from_command(
        const int8_t & command,
        const std::string& parameter, 
        Socket &skt,
        bool *was_closed);
    /**
     * Devuelve un vector con los datos recibidos 
     * para imprimir el estado de la partida
    */
    std::string wait_response(const std::string &data, Socket &skt);

    /**
     * Recibe el comando que fue ejecutado, y según este, 
     * recibe la cantidad de bytes esperados a través del
     * socket y devuelve la respuesta del servidor
    */
    std::string receive_response_from_command(
        char *buf, 
        const int8_t &command, 
        Socket &skt);

    /**
     * Devuelve el tipo de comando de mensaje según el string recibido
    */
    CommandType get_command_type(const std::string& resource);
};
#endif
