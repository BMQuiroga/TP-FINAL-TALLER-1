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

ProtocolResponse CProtocol::get(Socket &s, bool *was_closed) {
    ProtocolResponse resp;
    receive_number(&resp.content_type, s, was_closed);
    receive_number(&resp.size, s, was_closed);
    // std::cout << "Content Type: " << std::to_string(resp.content_type) << 
    // std::endl;
    // std::cout << "Response size: " << std::to_string(resp.size) << std::endl;
    if (resp.size > 0) {
        resp.content = std::vector<int8_t>(resp.size);
        s.recvall(resp.content.data(), resp.size, was_closed);
    }
    return resp;
}



void CProtocol::send(
    Socket &skt, 
    const ProtocolRequest &request, 
    bool was_closed
) {
     // TODO
}

void CProtocol::send_command(Intention& command, Socket &s, bool *was_closed) {
    int command_id = command.get_intention();
    // send_one_byte(command_id, s);
    send_number(command_id, s, was_closed);
}

void CProtocol::send_lobby_command(
    const LobbyCommand &command, 
    Socket &s, 
    bool *was_closed
) {
    Serializer serializer;
    ProtocolRequest req;
    GameReference ref;
    req.cmd = get_command_type(command.name);
    if (command.name == JOINGAME) {
        ref.id = (uint32_t) std::stoi(command.parameter);
    } else if (command.name == INPUTNAME) {
        serializer.push_string(req.content, command.parameter);
        serializer.push_number(req.content, command.parameter2);
        serializer.push_number(req.content, command.parameter3);
    } else if (command.name == CREATEGAME) {
        ref.id = -1;
        ref.name = command.parameter;
        ref.players = (uint8_t) command.parameter2;
    }

    if (req.cmd == JOIN || req.cmd == CREATE)
        req.content = serializer.serialize(ref);

    int bytes_sent = 0;
    bytes_sent += send_number(req.cmd, s, was_closed);
    if (!req.content.empty()) {
        bytes_sent += send_number((uint16_t)req.content.size(), s, was_closed);
        bytes_sent += s.sendall(
            req.content.data(), req.content.size(), was_closed);
    }
    // std::cout << "Content size: " << 
    // std::to_string(req.content.size()) << std::endl;
    // std::cout << "Sent " << 
    // std::to_string(bytes_sent) <<" to server" << std::endl;
}

uint8_t* CProtocol::receive_render(Socket &s) {
    uint8_t length;
    s.recvall(&length,1);
    uint8_t * render = new uint8_t[length+1];//podria ser unique ptr
    render[0] = length;
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
