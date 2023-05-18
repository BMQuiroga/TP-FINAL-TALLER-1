#include "protocol.h"
#include <arpa/inet.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <tuple>
#include "../server_src/matchstate.h"
#include <vector>
#include <cstdint>

void Protocol::send_number(
    const uint32_t &n, Socket &skt, bool *was_closed) {
    uint32_t number = htonl(n);
    skt.sendall(&number, sizeof(number), was_closed);
}

void Protocol::send_number(
    const uint16_t &n, Socket &skt, bool *was_closed) {
    uint16_t number = htons(n);
    skt.sendall(&number, sizeof(number), was_closed);
}

std::string Protocol::receive_text_message(
    Socket &peer, size_t size, bool *was_closed)
{
    char text_message[32] = {0};
    peer.recvall(&text_message, size, was_closed);
    return std::string(text_message);
}
