#include "client.h"
#include "../utils.h"
#include <thread>
#include <utility>
const int QUEUE_MAXSIZE = 10000;

Client::Client(
    Socket &&socket,
    ServerProtocol& protocol, 
    GameHandler &game_handler) :
    skt(std::move(socket)),
    responses(QUEUE_MAXSIZE),
    game_handler(game_handler),
    joined_game(0),
    uuid(get_uuid()),
    receiver(std::ref(skt), std::ref(responses), protocol),
    sender(std::ref(skt), std::ref(responses), protocol)
{
    receiver.register_callback([this](ProtocolRequest &message) {
        handle_request(message);
    });
}

void Client::handle_request(ProtocolRequest &message) {
    if (joined_game >= 0) {
        game_handler.get_game(joined_game).push_event(message, uuid, std::ref(responses));
    } else {
        // TODO: If this is a create game request, then create the 
        // game and return its initial state. If it's a join game request
        // then try to joint the game and return its current state on success
        // or 0 on failure.
    }
}

void Client::start()
{
    receiver.start();
    sender.start();
    /*ProtocolRequest req;
    req.cmd = JOIN;
    if (game_handler.get_game_count() == 0) {
        Game &game = game_handler.create_new_game("test", responses);
        game.start();
        // joined_game = game.get_id();
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
