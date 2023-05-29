#include <iostream>
#include "server_protocol.h"

uint8_t ServerProtocol::receive_command(Socket & s) {
    uint8_t n = -1;
    s.recvall(&n,ONE_BYTE);
    return n;
}

void ServerProtocol::send_render(char * data, int length, Socket & s) { 
    s.sendall(data,length);
}

ProtocolRequest ServerProtocol::get(Socket &skt, bool was_closed) {
    // get the client's request and return a 
    // ProtocolRequest representation of it
    ProtocolRequest request;
    int cmd = receive_command(skt);
    if (cmd >= 0) {
        request.move = cmd;
    }

    return request;
}

void ServerProtocol::send(Socket &skt, ProtocolResponse resp, bool was_closed) {
    send_number(resp.state, skt, &was_closed);
    send_number(resp.hit_points, skt, &was_closed);
    send_number(resp.position[0], skt, &was_closed);
    send_number(resp.position[1], skt, &was_closed);
}
