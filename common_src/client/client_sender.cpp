#include "client_sender.h"
#include <iostream>
#include <string>
#include "../queue.h"
#include <sstream>
#include <fstream>
#include <cstring>
#include <algorithm>

ClientSender::ClientSender(
    Socket& socket, 
    Queue<std::string>& q) :
    protocol(), 
    skt(socket),
    q(q)
{
}

void ClientSender::run() {
    std::string command;
    is_alive = keep_talking = true;
    while (keep_talking) {
        command = q.pop();
        if (command == "-1") {
            kill();
            break;
        }
        protocol.send_command(command, skt);
    }
}

bool ClientSender::is_dead()
{
    return not is_alive;
}

void ClientSender::kill()
{
    keep_talking = false;
}
