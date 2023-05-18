#include <iostream>
#include <exception>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <tuple>
#include <stdint.h>
#include <string>
#include <sstream>
#include <cstdint>
#include <cstdlib>
#include "queue.h"
#include "client_sender.h"

ClientSender::ClientSender(Socket &socket,
    Queue<MatchState> &q,
    ServerProtocol &protocol) :
    skt(socket),
    q(q),
    protocol(protocol)
{
}

void ClientSender::run()
{
    while (keep_talking) {
        MatchState state = q.pop();
        if (state.match_code == -1) {
            break;
        }
        print_message(state);
        protocol.send(skt, &state);
    }
}


void ClientSender::print_message(const MatchState &state)
{
    if (state.command == "create"){
        std::cout << "Created match: " << state.match_code << std::endl;
    } else if (state.command ==  "join") {
        if (state.succeeded) {
            std::cout << "Joined to match: " << state.match_code << std::endl;
        } else {
        std::cout << "Match does not exist: " << state.match_code << std::endl;
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
