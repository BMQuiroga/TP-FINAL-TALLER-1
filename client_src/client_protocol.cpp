#include "client_protocol.h"

#include <stdint.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>

ClientProtocol::ClientProtocol(
        const std::string& hostname,
        const std::string& servname) :
    hostname(hostname),  /* <-- construimos un `const std::string` */
    skt(hostname.c_str(), servname.c_str()) /* <-- construimos un `Socket` */
{
}

void ClientProtocol::async_get(const std::string& resource, 
std::vector<int> parameters) {
    bool was_closed = false;
    
    std::ostringstream request;
    request << resource;

    std::string buf = this->serialize(request.str(), 
    parameters);
    int8_t command = (int8_t) buf.c_str()[0];
    skt.sendall(&command, sizeof(command), &was_closed);

    if (buf.size() >= 2) {
        skt.sendall(&buf[1], sizeof(buf[1]), &was_closed);
    }

    if (buf.size() == 3) {
        skt.sendall(&buf[2], sizeof(buf[2]), &was_closed);
    }
}

std::vector<int> ClientProtocol::wait_response() {
    bool was_closed = false;
    char buf[7] = {0};
    skt.recvall(buf, sizeof(buf), &was_closed);
    std::vector<int> game_state = this->deserialize(buf);
    return game_state;
}


std::vector<int> ClientProtocol::get(
        const std::string& resource,
        const std::vector<int>& parameters) {
    async_get(resource, parameters);
    return wait_response();
}

GameState ClientProtocol::get_command_type(const std::string& resource) {
    if (resource == "Shooting") return GameState::SHOOT;
    if (resource == "Moving") return GameState::MOVE;
    if (resource == "Reload") return GameState::RELOAD;
    if (resource == "NOP") return GameState::NOP;
    return GameState::INVALID;
}

std::string ClientProtocol::serialize(const std::string& resource, 
std::vector<int> parameters) {
    GameState command = get_command_type(resource);
    std::ostringstream os;
    
    if (command == GameState::SHOOT) {
        os << (int8_t) command << (int8_t) parameters[0];
    } else if (command == GameState::MOVE) {
        os << (int8_t) command << (int8_t) parameters[0]
        << (int8_t) parameters[1];
    } else {
        os << (int8_t) command;
    }
    std::string result = os.str();

    return result;
}

std::vector<int> ClientProtocol::deserialize(char *data) {
    int8_t state = *(uint8_t*) data;
    uint16_t x = data[1] << 8 | data[2];
    uint16_t y = data[3] << 8 | data[4];
    uint16_t rounds = data[5] << 8 | data[6];

    std::vector<int> current_state {
        state,
        x,
        y,
        rounds};
    return current_state;
}
