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
    Queue<Intention*>& q) :
    protocol(), 
    skt(socket),
    q(q)
{
}

ClientSender::~ClientSender() {
    kill();
}

void ClientSender::run() {
    is_alive = keep_talking = true;
    bool was_closed = false;
    while (keep_talking) {
        try {
            Intention *command = q.pop();
            if (command->get_intention() == END) {
                kill();
                break;
            }
            protocol.send_command(*command, skt, &was_closed);
        } catch (...) {
            std::cout << "The sender was closed" << std::endl;
            kill();
        }
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
