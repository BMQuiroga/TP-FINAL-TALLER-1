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
    Queue<LobbyResponse> &q_responses) :
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
    while (keep_talking && !player_signed_up) {
        LobbyCommand command = q.pop();
        if (command.name == ENDLOBBY) {
            kill();
            break;
        }
        protocol.send_lobby_command(command, skt, &was_closed);
        if (command.name == JOINGAME || command.name == CREATEGAME) {
            Serializer serializer;
            LobbyResponse response;
            ProtocolResponse resp = protocol.get(skt, &was_closed);
            LobbyGameStateResponse lobby_response = serializer.deserialize_join_response(resp.content);
            response.game_state = lobby_response;
            q_responses.push(response);
            if (lobby_response.succeeded == 0) {
                break;
                player_signed_up = true;
            }
        } else if (command.name == GAMESLIST) {
            Serializer serializer;
            LobbyResponse response;
            ProtocolResponse resp = protocol.get(skt, &was_closed);
            LobbyGamesListsStateResponse lobby_response = serializer.deserialize_lobby_state(resp.content);
            response.games_list = lobby_response;
            q_responses.push(response);
        }
    }
    while (keep_talking) {
        Serializer serializer;
        ProtocolResponse resp = protocol.get(skt, &was_closed);
        LobbyGameStateResponse lobby_response = serializer.deserialize_join_response(resp.content);
        LobbyResponse response;
        response.game_state = lobby_response;
        q_responses.push(response);
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
