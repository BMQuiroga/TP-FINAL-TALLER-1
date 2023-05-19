#include "server_protocol.h"
#include <arpa/inet.h>
#include <tuple>
#include <vector>
#include <stdint.h>
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <cstdint>

ServerProtocol::ServerProtocol(
        const std::string& servname) :
    skt(servname.c_str()),
    peer(this->skt.accept())
{
}

std::tuple<std::string, std::vector<int>> ServerProtocol::receive(
    char *buf, bool was_closed) {
    /**
     * Recibo primero un solo byte para saber cual es el comando,
     * ver cuántos parámetros necesito recibir
    */    
    this->peer.recvall(buf, 1, &was_closed);
    auto command_name = deserialize_command_type(buf);
    /**
     * Recibo los parámetros necesarios según el comando que se recibe. 
    */
    std::vector<int> parameters = this->receive_paramaters(
        command_name, &was_closed);
    if (was_closed) {
        return std::make_tuple("", parameters);
    }
    return std::make_tuple(command_name, parameters);
}

std::vector<int> ServerProtocol::receive_paramaters(
    const std::string &command_name, bool *was_closed) {
    std::vector<int> parameters;
    if (command_name == "SHOOT") {
        char params[1] = {0};
        this->peer.recvall(params, sizeof(params), was_closed);
        parameters.push_back(params[0]);
    } else if (command_name == "MOVE") {
        char params[2] = {0};
        this->peer.recvall(params, sizeof(params), was_closed);
        parameters.push_back(params[0]);
        parameters.push_back(params[1]);
    }
    return parameters;
}

void ServerProtocol::send(const std::vector<int> resource) {
    bool was_closed = false;

    int8_t state = (int8_t) resource[0];
    uint16_t location_x = resource[1];
    uint16_t location_y = resource[2];
    uint16_t rounds = resource[3];

    location_x = htons(location_x);
    location_y = htons(location_y);
    rounds = htons(rounds);

    this->peer.sendall(&state, sizeof(state), &was_closed);
    this->peer.sendall(&location_x, sizeof(location_x), &was_closed);
    this->peer.sendall(&location_y, sizeof(location_y), &was_closed);
    this->peer.sendall(&rounds, sizeof(rounds), &was_closed);
}

/* Recibe datos del socket y los deserializa para
 * obtener el commando de juego a ejecutar y sus parametros*/
std::string ServerProtocol::deserialize_command_type(char *data) {
    int command_type = (int) (data[0]);
    std::string command_name;
    switch (command_type)
    {
    case 0: { command_name = "NOP"; } break;
    case 1: { command_name = "SHOOT"; } break;
    case 2: { command_name = "MOVE"; } break;
    case 3: { command_name = "RELOAD"; } break;
    }
    return command_name;
}
