#include <iostream>
#include "server_protocol.h"

uint8_t SProtocol::receive_command(Socket & s) {
    uint8_t n;
    s.recvall(&n,ONE_BYTE);
    return n;
}

void SProtocol::send_render(char * data, int length, Socket & s) { 
    s.sendall(data,length);
}
