#include "client.h"
#include <thread>
#include <utility>
const int QUEUE_MAXSIZE = 10000;

Client::Client(
    Socket &&socket,
    ServerProtocol& protocol, 
    GameHandler &game_handler) :
    skt(std::move(socket)),
    responses(QUEUE_MAXSIZE),
    receiver(std::ref(skt), std::ref(responses), protocol),
    sender(std::ref(skt), std::ref(responses), protocol)
{
    receiver.register_callback([this](ProtocolRequest &message) {
        handle_request(message);
    });
}

void Client::handle_request(const ProtocolRequest &message) {
    state.next_state(message);
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
}
