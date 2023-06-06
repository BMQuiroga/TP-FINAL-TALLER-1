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

ProtocolRequest ServerProtocol::get(Socket &skt, bool *was_closed) {
    // get the client's request and return a ProtocolRequest representation of it
    ProtocolRequest request;
    int cmd = receive_command(skt);
    if (cmd >= 0) {
        request.cmd = cmd;
    }

    if (cmd == CREATE) {
        // creo un CreateRequest en vez de un ProtocolRequest?
    } else if (cmd == JOIN) {
        // creo un JoinRequest?
    }

    return request;
}

void ServerProtocol::send(Socket &skt, ProtocolResponse resp, bool was_closed) {
    int bytes_sent = 0;
    bytes_sent += send_number(resp.content_type, skt, &was_closed);
    bytes_sent += send_number(resp.size, skt, &was_closed);
    bytes_sent += skt.sendall(resp.content.data(), resp.content.size(), &was_closed);
    std::cout << "sent " << std::to_string(bytes_sent) << " bytes to client" << std::endl;
}
