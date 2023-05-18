#ifndef SERVERPROTOCOL_H
#define SERVERPROTOCOL_H
#include <stdint.h>
#include <iostream>
#include "../common_src/socket.h"
#include "../server_src/matchstate.h"
#include "../common_src/protocol.h"
#include <string>
#include <vector>
#include <sstream>
#include <tuple>

class ServerProtocol: public Protocol {
    private:
    void send_success(
        bool succeeded, 
        Socket &peer, 
        bool *was_closed);
        
    public:
    ServerProtocol() = default;

    /**
     * Envía el estado de la partida a través del socket
    */
    void send(Socket &peer, MatchState *data);
    /**
     * Recibe los datos a través del socket y devuelve el comando
     * y los parámetros recibidos
    */
    std::tuple<std::string, std::string> receive(
        Socket &peer, 
        char *buf, 
        bool was_closed);
    /**
     * Recibe la cantidad de parametros necesarios según el 
     * tipo de comando y devuelve un vector con los parámetros recibidos
    */    
    std::string receive_paramaters(
        Socket &peer,
        int num_parameters, 
        bool *was_closed);
    /**
     * Deserializa los bytes recibidos del socket y convierte 
     * los datos a una tupla con el comando recibido y el número
     *  de parámetros que requiere
    */
    std::tuple<std::string, int> deserialize_command_type(
        char *data);
};
#endif
