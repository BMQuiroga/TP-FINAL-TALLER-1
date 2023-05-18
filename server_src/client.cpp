#include "client.h"
#include <thread>
#include <utility>
const int QUEUE_MAXSIZE = 10000;

Client::Client(
    Socket &&socket,
    ServerProtocol& protocol, 
    GameHandler &game_handler) :
    skt(std::move(socket)),
    q(QUEUE_MAXSIZE),
    th_r(std::ref(skt), std::ref(q), protocol, game_handler),
    /**
     * Raro q le pases a los dos threads la misma cola...
    */
    th_s(std::ref(skt), std::ref(q), protocol)
{
}

void Client::start()
{
    th_r.start();
    th_s.start();
}

void Client::join()
{
    th_r.join();
    th_s.join();
}

bool Client::is_dead()
{
    return th_r.is_dead() || th_s.is_dead();
}

void Client::kill()
{
    skt.shutdown(0);
    skt.close();
    th_r.kill();
    th_s.kill();
}
