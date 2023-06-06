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
    receive_number(&resp.content_type, s, was_closed);
    receive_number(&resp.size, s, was_closed);
    std::cout << "Content Type: " << std::to_string(resp.content_type) << std::endl;
    std::cout << "Response size: " << std::to_string(resp.size) << std::endl;
    resp.content = std::vector<int8_t>(resp.size);
    s.recvall(resp.content.data(), resp.size, was_closed);
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
