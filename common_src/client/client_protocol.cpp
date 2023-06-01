#include <iostream>
#include "client_protocol.h"
#include <string>
#include "intention.h"
//#include <arpa/inet.h>

void CProtocol::send_one_byte(uint8_t n, Socket &s) {
    s.sendall(&n,ONE_BYTE);
}

/*void foo(Socket &s, bool *was_closed, ProtocolResponse &resp) {
    PlayerStateReference ref;
    uint8_t q;
    //uint16_t k;
    s.recvall(&q, 1, was_closed);
    ref.id = q;
    //std::cout << q << "<-" << std::endl;
    s.recvall(&ref.x, sizeof(ref.x), was_closed);
    s.recvall(&ref.y, sizeof(ref.y), was_closed);
    s.recvall(&ref.direction, sizeof(ref.direction), was_closed);
    // s.recvall(&ref.rounds, sizeof(ref.rounds), was_closed);
    s.recvall(&ref.state, sizeof(ref.state), was_closed);
    s.recvall(&ref.hit_points, sizeof(ref.hit_points), was_closed);
    resp.players.push_back(ref);
    std::cout << ref.id << "---" <<ref.state << std::endl;}*/

ProtocolResponse CProtocol::get(Socket &s, bool *was_closed) {
    ProtocolResponse resp;
    uint16_t bytes;
    receive_number(&bytes, s, was_closed);
    int iter = bytes / 8;
    for (int i = 0; i < iter; i++) {
        PlayerStateReference player;
        receive_number(&player.id, s, was_closed);
        receive_number(&player.x, s, was_closed);
        receive_number(&player.y, s, was_closed);
        receive_number(&player.direction, s, was_closed);
        receive_number(&player.state, s, was_closed);
        receive_number(&player.hit_points, s, was_closed);
        std::cout << "Player: " << std::endl << 
            "- name: " << player.name << std::endl <<
            "- state: " << std::to_string(player.state) << std::endl <<
            "- hit points: " << std::to_string(player.hit_points) << std::endl <<
            "- x: " << std::to_string(player.x) << std::endl <<
            "- y: " << std::to_string(player.y) << std::endl;
        resp.players.push_back(player);
    }
    receive_number(&resp.game_state, s, was_closed);
    std::cout << "Game state: " << std::to_string(resp.game_state) << std::endl;
    std::cout << "ended CProtocol get" << std::endl;
    return resp;
}



void CProtocol::send(Socket &skt, ProtocolRequest request, bool was_closed) {
     // TODO
}

void CProtocol::send_command(Intention& command, Socket &s, bool *was_closed) {
    uint8_t command_id = (uint8_t) command.get_intention();
    send_one_byte(command_id, s);
    //send_number(command_id, s, was_closed);
}

uint8_t* CProtocol::receive_render(Socket &s) {
    uint8_t length;
    s.recvall(&length,1);
    uint8_t * render = new uint8_t[length+1];//podria ser unique ptr
    uint8_t * render_plus_one = render++;
    s.recvall(render_plus_one,length);
    render[0] = length;
    return render;
}
