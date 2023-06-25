#include "client.h"
#include "../utils.h"
#include <thread>
#include <utility>
#include "../serialization.h"
const int QUEUE_MAXSIZE = 10000;

Client::Client(
    Socket &&socket,
    ServerProtocol& protocol, 
    GameHandler &game_handler) :
    skt(std::move(socket)),
    responses(QUEUE_MAXSIZE),
    game_handler(game_handler),
    joined_game(-1),
    name(""),
    uuid(get_uuid()),
    receiver(std::ref(skt), /*std::ref(responses),*/ protocol),
    sender(std::ref(skt), std::ref(responses), protocol)
{
    receiver.register_callback([this](ProtocolRequest &message) {
        handle_request(message);
    });
}

void Client::handle_request(ProtocolRequest &message) {
    if (joined_game >= 0) {
        game_handler.get_game(joined_game).push_event(message, name, weapon_code, std::ref(responses));
        if (message.cmd < 0) {
            kill();
        }
    } else {
        Serializer serializer;
        // If this is a create game request, then create the 
        // game and return its initial state. If it's a join game request
        // then try to join the game and return its current state on success
        // or 1 on failure.
        if (message.cmd == CREATE) {
            LobbyGameStateResponse resp;
            ProtocolResponse response;
            response.content_type = CREATE;
            GameReference create_req = serializer.deserialize_game_reference(message.content);
            Game &game = game_handler.create_new_game(create_req, responses);
            game.start();
            game_handler.join_game(game.get_id(), name, weapon_code, responses);
            joined_game = game.get_id();
            resp.game_code = joined_game;
            resp.succeeded = JOIN_SUCCESS;
            response.content = serializer.serialize(resp);
            response.size = response.content.size();
            responses.push(std::ref(response));
        }
        if (message.cmd == LIST) {
            LobbyStateResponse resp;
            resp.games = game_handler.get_games().get_refs();
            ProtocolResponse response;
            response.content_type = LOBBY_STATE;
            response.content = serializer.serialize(resp);
            responses.push(response);
        }
        if (message.cmd == JOIN) {
            JoinRequest join_req = serializer.deserialize_join_state(message.content);
            ProtocolResponse response;
            LobbyGameStateResponse resp; 
            int join_code = game_handler.join_game(join_req.game_code, name, weapon_code, responses);
            joined_game = join_code;
            resp.game_code = joined_game;
            resp.succeeded = JOIN_SUCCESS;
            if (join_code == JOIN_FAILURE) {
                joined_game = -1;
                resp.succeeded = JOIN_FAILURE;
            }
            response.content_type = JOIN;
            response.content = serializer.serialize(resp);
            response.size = response.content.size();
            responses.push(std::ref(response));
        }
        if (message.cmd == PLAYERNAME) {
            NewPlayerRequest new_player_req = serializer.deserialize_input_name(message.content);
            name = new_player_req.name;
            weapon_code = new_player_req.weapon_code;
            game_mode = new_player_req.game_mode;
            std::cout << "game mode is " << game_mode << std::endl;
        }
    }
}

void Client::start()
{
    receiver.start();
    sender.start();
}

void Client::join()
{
    receiver.join();
    sender.join();
}

bool Client::is_dead()
{
    return receiver.is_dead() || sender.is_dead();
}

void Client::kill()
{
    skt.shutdown(0);
    skt.close();
    receiver.kill();
    sender.kill();
    responses.close();
}
