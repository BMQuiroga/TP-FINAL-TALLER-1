#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "server_protocol.h"
#include "../serialization.h"

uint8_t ServerProtocol::receive_command(Socket & s) {
    uint8_t n = -1;
    s.recvall(&n,ONE_BYTE);
    return n;
}

void ServerProtocol::send_render(char * data, int length, Socket & s) { 
    s.sendall(data,length);
}

ProtocolRequest ServerProtocol::get(Socket &skt, bool *was_closed) {
    // get the client's request and return a ProtocolRequest representation of it
    ProtocolRequest request;
    uint16_t size;
    receive_number(&request.cmd, skt, was_closed);
    if (request.cmd == CREATE || request.cmd == JOIN || request.cmd == PLAYERNAME) {
        receive_number(&size, skt, was_closed);
        request.content = std::vector<int8_t>(size);
        skt.recvall(request.content.data(), size, was_closed);
    }
    return request;
}

void ServerProtocol::send(Socket &skt, const ProtocolResponse &resp, bool was_closed) {
    int bytes_sent = 0;
    bytes_sent += send_number(resp.content_type, skt, &was_closed);
    if (!resp.content.empty()) {
        bytes_sent += send_number(resp.size, skt, &was_closed);
        bytes_sent += skt.sendall(resp.content.data(), resp.content.size(), &was_closed);
    }
    std::cout << "sent " << std::to_string(bytes_sent) << " bytes to client" << std::endl;
}
