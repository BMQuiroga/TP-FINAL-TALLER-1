#include <arpa/inet.h>

#include <stdint.h>
#include <iostream>
#include "../socket.h"
#include <string>
#include <vector>
#include <sstream>

enum GameState {
    NOP = 0x00,
    SHOOT = 0x01,
    MOVE = 0x02,
    RELOAD = 0x03,
    INVALID = -1
};

/*
 * Versión simplificada del protocolo HTTP desde el punto de vista
 * de un cliente HTTP (no del server).
 * */
class ClientProtocol {
    private:
    const std::string hostname;
    Socket skt;

    public:
    explicit ClientProtocol(
            const std::string& hostname,
            const std::string& servname);

    /**
     * Recibe el comando y los parametros necesarios 
     * para que se ejecute.
    */
    void async_get(const std::string& resource, 
    std::vector<int> parameters);

    /**
     * Devuelve un vector con los datos recibidos 
     * para imprimir el estado del juego
    */
    std::vector<int> wait_response();

    std::vector<int> get(const std::string& resource, 
    const std::vector<int>& parameters);
    /*
     * No queremos permitir que alguien haga copias
     * */
    ClientProtocol(const ClientProtocol&) = delete;
    ClientProtocol& operator=(const ClientProtocol&) = delete;

    ClientProtocol(ClientProtocol&&) = default;
    ClientProtocol& operator=(ClientProtocol&&) = default;

    /**
     * Convierte el comando recibido y sus parametros a un 
     * string de bytes para enviar a través del socket 
    */
    std::string serialize(const std::string& resource, 
    std::vector<int> parameters);

    /**
     * Convierte los datos recibidos del socket a un vector
     * con el estado del juego y devuelve el estado
    */
    std::vector<int> deserialize(char *data);

    /**
     * Devuelve el tipo de comando de juego según el string recibido
    */
    GameState get_command_type(const std::string& resource);
};
