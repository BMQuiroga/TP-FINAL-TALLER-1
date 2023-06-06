#include <iostream>
#include "client_protocol.h"
#include <string>
#include <iostream>
#include <sstream>
#include "intention.h"
#include "../qt/lobby_command.h"
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

void CProtocol::send_lobby_command(const LobbyCommand &command, Socket &s, bool *was_closed)
{
    uint8_t command_id = (uint8_t) get_command_type(command.name);
    send_one_byte(command_id, s);
    if (command.name == JOINGAME) {
        uint32_t game_code = (uint32_t) std::stoi(command.parameter);
        send_number(game_code, s, was_closed);
    } else {
        std::ostringstream ss;
        uint16_t len = (uint16_t) command.parameter.length();
        send_number(len, s, was_closed);
        ss << command.parameter;
        auto buf = ss.str();
        s.sendall(buf.data(), buf.size(), was_closed);
    } 
    if (command.name == CREATEGAME) {
        uint8_t game_size = (uint8_t) command.parameter2;
        send_one_byte(game_size, s);
    }
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

int CProtocol::get_command_type(const std::string &resource) {
    if (resource == CREATEGAME) return CREATE;
    if (resource == JOINGAME) return JOIN;
    if (resource == INPUTNAME) return PLAYERNAME;
    return INVALID;
}
