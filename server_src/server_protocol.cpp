#include "server_protocol.h"
#include <arpa/inet.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <tuple>
#include "../server_src/matchstate.h"
#include <vector>
#include <cstdint>

void ServerProtocol::send(Socket &peer, MatchState *data) {
    bool was_closed = false;
    if (data->command == "create") {
        uint32_t response = (uint32_t) data->match_code;
        send_number(response, peer, &was_closed);
    } else if (data->command == "join") {
        send_success(data->succeeded, peer, &was_closed);
    } else if (data->command ==  "broadcast") {
        std::ostringstream ss;
        ss << data->message;
        auto buf = ss.str();
        int8_t state = 0x04;
        peer.sendall(&state, sizeof(state), &was_closed);
        uint16_t message_len = (uint16_t) data->message.length();
        send_number(message_len, peer, &was_closed);
        peer.sendall(buf.data(), buf.size(), &was_closed);
    }
}

std::tuple<std::string, std::string> ServerProtocol::receive(
    Socket &peer, char *buf, bool was_closed) {
    /**
     * Recibo primero un solo byte para saber cual es el comando,
     * veo cuántos parámetros necesito recibir
    */    
    peer.recvall(buf, 1, &was_closed);
    if (was_closed) {
        return std::make_tuple("", "");
    }
    auto command_info = deserialize_command_type(buf);
    std::string command_name = std::get<0>(command_info);
    int num_paramaters = std::get<1>(command_info);
    /**
     * Recibo los parámetros necesarios según el comando que se recibe. 
    */
    std::string parameter = this->receive_paramaters(
        peer, num_paramaters, &was_closed);
   
    return std::make_tuple(command_name, parameter);
}

std::string ServerProtocol::receive_paramaters(
    Socket &peer, int num_parameters, bool *was_closed) {
    std::ostringstream parameter;
    char params[4] = {0};
    if (num_parameters == 1) {
        uint32_t code = recv_number<uint32_t>(
            peer, params, sizeof(params), was_closed);
        parameter << code;
    } else if (num_parameters == 2) {
        uint16_t len_param = recv_number<uint16_t>(
            peer, params, num_parameters, was_closed);
        parameter << receive_text_message(peer, len_param, was_closed);;
    }
    return parameter.str();
}

/**
 * Recibe datos del socket y los deserializa para obtener
 * el commando de juego a ejecutar y sus parametros
*/
std::tuple<std::string, int> ServerProtocol::deserialize_command_type(
    char *data) {
    int command_type = (int) (data[0]);
    std::string command_name;
    int num_parameters;
    switch (command_type)
    {
    case 1: { 
        command_name = "create"; 
        num_parameters = 2;
    } break;
    case 2: { 
        command_name = "join";
        num_parameters = 1;
    } break;
    case 3: { 
        command_name = "broadcast";
        num_parameters = 2;
    } break;
    }
    return std::make_tuple(command_name,num_parameters);
}

void ServerProtocol::send_success(
    bool succeeded, Socket &peer, bool *was_closed) {
    int8_t response = succeeded ? (int8_t) 0x00 : (int8_t) 0x01;
    peer.sendall(&response, 1, was_closed);
}
