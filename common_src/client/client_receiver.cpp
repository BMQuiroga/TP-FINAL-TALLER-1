#include "client_receiver.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include "../queue.h"

ClientReceiver::ClientReceiver(
    Socket& socket,
    Queue<std::list<std::string>>& q) :
    protocol(), 
    skt(socket),
    q(q)
{
}

void ClientReceiver::run() {
    std::string command;
    is_alive = keep_talking = true;
    while (keep_talking) {
        uint8_t * resp = protocol.get(skt, keep_talking);
        //q.push(resp);
        if (!keep_talking) {
            break;
        }
    }
}

bool ClientReceiver::is_dead()
{
    return not is_alive;
}

void ClientReceiver::kill()
{
    keep_talking = false;
}
