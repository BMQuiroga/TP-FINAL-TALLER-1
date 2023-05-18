#include "client_protocol.h"
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


void ClientProtocol::send_command(
    const std::string &data, 
    const std::string &parameter,
    Socket &skt) {
    bool was_closed = false;
    int8_t command = (int8_t) get_command_type(data);
    skt.sendall(&command, sizeof(command), &was_closed);
    send_info_from_command(command, parameter, skt, &was_closed);
}

void ClientProtocol::send_info_from_command(
    const int8_t &command, 
    const std::string &parameter, 
    Socket &skt, bool *was_closed) {
    if (command == JOIN) {
        uint32_t game_code = (uint32_t) std::stoi(parameter);
        send_number(game_code, skt, was_closed);
    } else {
        std::ostringstream ss;
        uint16_t len = (uint16_t) parameter.length();
        send_number(len, skt, was_closed);
        ss << parameter;
        auto buf = ss.str();
        skt.sendall(buf.data(), buf.size(), was_closed);
    }
}

std::string ClientProtocol::wait_response(
    const std::string &data, Socket &skt) {
    int8_t command = (int8_t) get_command_type(data);
    char buf[4] = {0};
    auto response = receive_response_from_command(buf, command, skt);
    return response;
}

std::string ClientProtocol::receive_response_from_command(
    char *buf, const int8_t &command, Socket &skt) {    
    bool was_closed = false;
    std::ostringstream response;
    if (command == CREATE) {
        uint32_t code = recv_number<uint32_t>(skt, buf, 4, &was_closed);
        response << code;
    } else if (command == JOIN) {
        skt.recvall(buf, 1, &was_closed);
        int8_t code = buf[0];
        response << (int) code;
    } else if (command == BROADCASTED) {
        skt.recvall(buf, 1, &was_closed);
        uint16_t len = recv_number<uint16_t>(skt, buf, 2, &was_closed);
        response << receive_text_message(skt, len, &was_closed);
    }
    return response.str();
}

CommandType ClientProtocol::get_command_type(const std::string &resource) {
    if (resource == "create") return CREATE;
    if (resource == "join") return JOIN;
    if (resource == "broadcast") return BROADCAST;
    if (resource == "read") return BROADCASTED;
    return INVALID;
}
