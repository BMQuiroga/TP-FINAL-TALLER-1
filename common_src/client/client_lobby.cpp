#include "client_lobby.h"
#include <iostream>
#include <string>
#include "../qt/lobby_command.h"
#include "../serialization.h"
#include "../queue.h"
#include <sstream>
#include <fstream>
#include <cstring>
#include <algorithm>

ClientLobby::ClientLobby(
    Socket& socket, 
    Queue<LobbyCommand>& q,
    Queue<LobbyGameStateResponse> &q_responses) :
    protocol(), 
    skt(socket),
    q(q),
    q_responses(q_responses)
{
}

ClientLobby::~ClientLobby()
{
}

void ClientLobby::run() {
    is_alive = keep_talking = true;
    bool was_closed = false;
    while (keep_talking) {
        LobbyCommand command = q.pop();
        if (command.name == ENDLOBBY) {
            kill();
            break;
        }
        protocol.send_lobby_command(command, skt, &was_closed);
        if (command.name == JOINGAME || command.name == CREATEGAME) {
            Serializer serializer;
            ProtocolResponse resp = protocol.get(skt, &was_closed);
            LobbyGameStateResponse lobby_response = serializer.deserialize_join_response(resp.content);
            q_responses.push(lobby_response);
            if (lobby_response.succeeded == 0) {
                break;
                player_signed_up = true;
            }
        }
    }
    while (keep_talking) {
        Serializer serializer;
        ProtocolResponse resp = protocol.get(skt, &was_closed);
        LobbyGameStateResponse lobby_response = serializer.deserialize_join_response(resp.content);
        q_responses.push(lobby_response);
        if (lobby_response.ready == 0) {
            kill();
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
