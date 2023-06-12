#include "client_lobby.h"
#include <iostream>
#include <string>
#include "../qt/lobby_command.h"
#include "../queue.h"
#include <sstream>
#include <fstream>
#include <cstring>
#include <algorithm>

ClientLobby::ClientLobby(
    Socket& socket, 
    Queue<LobbyCommand>& q) :
    protocol(), 
    skt(socket),
    q(q)
{
}

ClientLobby::~ClientLobby() {
    kill();
    join();
}

void ClientLobby::run() {
    is_alive = keep_talking = true;
    bool was_closed = false;
    while (keep_talking) {
        LobbyCommand command = q.pop();
        if (command.name == "end") {
            kill();
            break;
        }
        protocol.send_lobby_command(command, skt, &was_closed);
        if (command.name == "join") {
            ProtocolResponse resp = protocol.get(skt, &was_closed);
            std::cout << resp.size << std::endl;
        }
    }
}

bool ClientLobby::is_dead()
{
    return not is_alive;
}

void ClientLobby::kill()
{
    keep_talking = false;
}
