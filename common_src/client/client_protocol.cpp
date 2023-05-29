#include <iostream>
#include "client_protocol.h"
#include <string>
#include "intention.h"
//#include <arpa/inet.h>

void CProtocol::send_one_byte(uint8_t n, Socket &s) {
    s.sendall(&n,ONE_BYTE);
}

ProtocolResponse CProtocol::get(Socket &s, bool *was_closed) {
    // uint8_t length;
    // s.recvall(&length,1,&was_closed);
    // uint8_t * render = new uint8_t[length+1];//podria ser unique ptr
    // uint8_t * render_plus_one = render++;
    // s.recvall(render_plus_one,length,&was_closed);
    // render[0] = length;
    // return render;
    ProtocolResponse resp;
    size_t bytes;
    s.recvall(&bytes, sizeof(bytes), was_closed);
    int iter = bytes / sizeof(ProtocolResponse);
    for (int i = 0; i < iter; i++) {
        PlayerStateReference ref;
        s.recvall(&ref.id, sizeof(ref.id), was_closed);
        s.recvall(&ref.x, sizeof(ref.x), was_closed);
        s.recvall(&ref.y, sizeof(ref.y), was_closed);
        s.recvall(&ref.direction, sizeof(ref.direction), was_closed);
        // s.recvall(&ref.rounds, sizeof(ref.rounds), was_closed);
        s.recvall(&ref.state, sizeof(ref.state), was_closed);
        s.recvall(&ref.hit_points, sizeof(ref.hit_points), was_closed);
        resp.players.push_back(ref);
    }
    s.recvall(&resp.game_state, sizeof(resp.game_state), was_closed);
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
