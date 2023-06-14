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
        game_handler.get_game(joined_game).push_event(message, name, std::ref(responses));
    } else {
        Serializer serializer;
        // TODO: If this is a create game request, then create the 
        // game and return its initial state. If it's a join game request
        // then try to joint the game and return its current state on success
        // or 0 on failure.
        if (message.cmd == CREATE) {
            GameReference create_req = serializer.deserialize_game_reference(message.content);
            Game &game = game_handler.create_new_game(create_req.name, responses);
            game.start();
            game_handler.join_game(game.get_id(), name, responses);
            joined_game = game.get_id();
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
            std::cout << "game code is: " << join_req.game_code << std::endl;
            int join_code = game_handler.join_game(join_req.game_code, name, responses);
            joined_game = join_code;
            if (join_code == JOIN_FAILURE) {
                ProtocolResponse resp;
                resp.content_type = JOIN;
                serializer.push_number(resp.content, JOIN_FAILURE);
                resp.size = resp.content.size();
                responses.push(std::ref(resp));
            }
        }
        if (message.cmd == PLAYERNAME) {
            InputNameRequest name_req = serializer.deserialize_input_name(message.content);
            name = name_req.name;
        }
    }
}

void Client::start()
{
    receiver.start();
    sender.start();
    /*
    ProtocolRequest req;
    req.cmd = JOIN;
    if (game_handler.get_game_count() == 0) {
        Game &game = game_handler.create_new_game("test", responses);
        game.start();
        joined_game = game.get_id();
        std::cout << "Created game with id: " << std::to_string(joined_game) << std::endl;
        game.push_event(std::ref(req), uuid, responses);
    } else {
        Game &game = game_handler.get_game(joined_game);
        game.push_event(std::ref(req), uuid, responses);
    }
    std::cout << "Pushing event: " << "(req=JOIN, name=" << uuid << ")" << std::endl;
    */
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
}
