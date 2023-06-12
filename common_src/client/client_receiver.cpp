#include "client_receiver.h"
#include "../liberror.h"
#include <iostream>
#include <exception>
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
ClientReceiver::~ClientReceiver() {
    kill();
    join();
}

void ClientReceiver::run() {
    ProtocolResponse resp;
    is_alive = keep_talking = true;
    bool was_closed = false;
    while (keep_talking) {
        try {
            resp = protocol.get(skt, &was_closed);
            if (was_closed) {
                kill();
            }
            q.push(resp);
        } catch (const LibError& err) {
            if (std::string(err.what()).compare(
                "socket recv failedBad file descriptor") == 0) {
                kill();
                break;
            }
            throw;
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
