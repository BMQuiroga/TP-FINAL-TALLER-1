#include <iostream>
#include "server_protocol.h"
#include <arpa/inet.h>

void SProtocol::listen(std::string & msg, Socket &s) {
    uint8_t code;//medio al pedo, siempre es 3
    s.recvall(&code,ONE_BYTE);
    uint16_t size;
    s.recvall(&size,TWO_BYTES);
    size = ntohs(size);
    char* list = new char[size];
    s.recvall(list,size);
    msg = list;
    delete[] list;
}

uint8_t SProtocol::recv_command(Socket &s) {
    uint8_t byte;
    s.recvall(&byte,ONE_BYTE);
    return byte;
}

void SProtocol::send_replycode(uint8_t byte, Socket &s) {
    s.sendall(&byte,ONE_BYTE);
}

uint32_t SProtocol::recv_code(Socket &s) {
    uint32_t bytes;
    s.recvall(&bytes,FOUR_BYTES);
    return ntohl(bytes);
}

void SProtocol::handle_create(Socket &s) {
    uint16_t bytes;
    s.recvall(&bytes,TWO_BYTES);
    bytes = ntohs(bytes);
    char* msg = new char[bytes];
    s.recvall(msg,bytes);
    delete[] msg;
}

void SProtocol::send_code(uint32_t bytes, Socket &s) {
    bytes = htonl(bytes);
    s.sendall(&bytes,FOUR_BYTES);
}

void SProtocol::sendmsg(std::string msg, Socket &s) {
    uint8_t msgcode = COMMAND_BROADCAST;
    s.sendall(&msgcode,ONE_BYTE);
    uint16_t msglength = msg.size();
    uint16_t msglengthsend = ntohs(msglength);
    s.sendall(&msglengthsend,TWO_BYTES);
    const char *cstr = msg.c_str();
    s.sendall(cstr,msglength);
}
