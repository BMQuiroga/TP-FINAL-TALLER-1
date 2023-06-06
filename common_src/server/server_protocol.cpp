#include <iostream>
#include <string>
#include <sstream>
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
    Serializer serializer;
    request.cmd = receive_command(skt);
    uint16_t size;
    skt.recvall(&size, sizeof(size), was_closed);
    skt.recvall(request.content.data(), size, was_closed);
    // std::ostringstream parameter;
    // char params[4] = {0};
    // if (cmd >= 0) {
    //     request.cmd = cmd;
    // }

    // if (cmd == PLAYERNAME) {
    //     parameter << receive_text_message(skt, was_closed);
    // } else if (cmd == JOIN) {
    //     uint32_t code = recv_number<uint32_t>(
    //         skt, params, sizeof(params), was_closed);
    //     parameter << code;
    // } else if (cmd == CREATE) {
    //     uint8_t max_players = receive_command(skt);
    //     parameter << max_players;
    //     parameter << receive_text_message(skt, was_closed);
    // } else {
    //     return request;
    // }
    // serializer.push_string(request.content, parameter.str());
    // return request;
}

void ServerProtocol::send(Socket &skt, const ProtocolResponse &resp, bool was_closed) {
    int bytes_sent = 0;
    bytes_sent += send_number(resp.content_type, skt, &was_closed);
    bytes_sent += send_number(resp.size, skt, &was_closed);
    bytes_sent += skt.sendall(resp.content.data(), resp.content.size(), &was_closed);
    std::cout << "sent " << std::to_string(bytes_sent) << " bytes to client" << std::endl;
}
