#include <stdint.h>
#include <iostream>
#include <tuple>
#include "../socket.h"
#include <string>
#include <sstream>
#include <vector>


class ServerProtocol {
    private:
    Socket skt;
    Socket peer;

    public:
    explicit ServerProtocol(
            const std::string& servname);

    /*
     * No queremos permitir que alguien haga copias
     * */
    ServerProtocol(const ServerProtocol&) = delete;
    ServerProtocol& operator=(const ServerProtocol&) = delete;

    ServerProtocol(ServerProtocol&&) = default;
    ServerProtocol& operator=(ServerProtocol&&) = default;

    /**
     * Envía el estado del juego a través del socket
    */
    void send(std::vector<int> resource);
    /**
     * Recibe los datos a través del socket y devuelve el comando
     * y los parámetros recibidos
    */
    std::tuple<std::string, std::vector<int>> receive(
        char *buf,
        bool was_closed);
    /**
     * Recibe la cantidad de parametros necesarios según el 
     * tipo de comando y devuelve un vector con los parámetros recibidos
    */
    std::vector<int> receive_paramaters(
        const std::string &command_name, 
        bool *was_closed);
    /**
     * Deserializa los bytes recibidos del socket y devuelve
     * un string con el nombre del comando
    */
    std::string deserialize_command_type(char *data);
};
