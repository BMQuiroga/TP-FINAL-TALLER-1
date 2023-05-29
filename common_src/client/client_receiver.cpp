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
    Queue<ProtocolResponse>& q) :
    protocol(), 
    skt(socket),
    q(q)
{
}

void ClientReceiver::run() {
    ProtocolResponse resp;
    is_alive = keep_talking = true;
    bool was_closed = false;
    while (keep_talking) {
        resp = protocol.get(skt, &was_closed);
        if (!was_closed) {
            kill();
        }
        q.push(resp);
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
