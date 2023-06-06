#include <iostream>
#include "client_protocol.h"
#include <string>
#include <iostream>
#include <sstream>
#include "intention.h"
#include <vector>
#include "../serialization.h"
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
    receive_number(&resp.content_type, s, was_closed);
    receive_number(&resp.size, s, was_closed);
    std::cout << "Content Type: " << std::to_string(resp.content_type) << std::endl;
    std::cout << "Response size: " << std::to_string(resp.size) << std::endl;
    resp.content = std::vector<int8_t>(resp.size);
    s.recvall(resp.content.data(), resp.size, was_closed);
    std::cout << "ended CProtocol get" << std::endl;
    return resp;
}



void CProtocol::send(Socket &skt, const ProtocolRequest &request, bool was_closed) {
     // TODO
}

void CProtocol::send_command(Intention& command, Socket &s, bool *was_closed) {
    uint8_t command_id = (uint8_t) command.get_intention();
    send_one_byte(command_id, s);
    //send_number(command_id, s, was_closed);
}

void CProtocol::send_lobby_command(const LobbyCommand &command, Socket &s, bool *was_closed)
{
    Serializer serializer;
    ProtocolRequest req;
    GameReference ref;
    req.cmd = (uint8_t) get_command_type(command.name);
    if (command.name == JOINGAME) {
        ref.id = (uint32_t) std::stoi(command.parameter);
    } else if (command.name == INPUTNAME) {
        std::ostringstream ss;
        uint16_t len = (uint16_t) command.parameter.length();
        send_number(len, s, was_closed);
        ss << command.parameter;
        auto buf = ss.str();
        s.sendall(buf.data(), buf.size(), was_closed);
    } else if (command.name == CREATEGAME) {
        ref.id = -1;
        ref.name = command.parameter;
        ref.players = (uint8_t) command.parameter2;
    }

    req.content = serializer.serialize(ref);
    int bytes_sent = 0;
    bytes_sent += send_number(req.cmd, s, was_closed);
    bytes_sent += send_number((uint16_t)req.content.size(), s, was_closed);
    bytes_sent += s.sendall(req.content.data(), req.content.size(), was_closed);

    // uint8_t command_id = (uint8_t) get_command_type(command.name);
    // send_one_byte(command_id, s);
    // if (command.name == JOINGAME) {
    //     uint32_t game_code = (uint32_t) std::stoi(command.parameter);
    //     send_number(game_code, s, was_closed);
    // } else {
    //     std::ostringstream ss;
    //     uint16_t len = (uint16_t) command.parameter.length();
    //     send_number(len, s, was_closed);
    //     ss << command.parameter;
    //     auto buf = ss.str();
    //     s.sendall(buf.data(), buf.size(), was_closed);
    // } 
    // if (command.name == CREATEGAME) {
    //     uint8_t game_size = (uint8_t) command.parameter2;
    //     send_one_byte(game_size, s);
    // }
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
