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

    return request;
}

void ServerProtocol::send(Socket &skt, ProtocolResponse resp, bool was_closed) {
    int bytes_sent = 0;
    bytes_sent += send_number(resp.players.size()*8, skt, &was_closed);//8 BYTES POR JUGADOR
    for (auto player : resp.players) {
        std::cout << "Player: " << std::endl << 
            "- name: " << player.name << std::endl <<
            "- state: " << std::to_string(player.state) << std::endl <<
            "- hit points: " << std::to_string(player.hit_points) << std::endl <<
            "- x: " << std::to_string(player.x) << std::endl <<
            "- y: " << std::to_string(player.y) << std::endl;
        bytes_sent += send_number(player.id, skt, &was_closed);//como no hay soldado 4, no permite 4 jugadores
        bytes_sent += send_number(player.x, skt, &was_closed);
        bytes_sent += send_number(player.y, skt, &was_closed);
        bytes_sent += send_number(player.direction, skt, &was_closed);     
        bytes_sent += send_number(player.state, skt, &was_closed);
        bytes_sent += send_number(player.hit_points, skt, &was_closed);
    }
    bytes_sent += send_number(resp.game_state, skt, &was_closed);
    std::cout << "sent " << std::to_string(bytes_sent) << " bytes to client" << std::endl;
}
