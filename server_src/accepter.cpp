#include "accepter.h"
#include "../common_src/socket.h"
#include <iostream>
#include <exception>
#include <fstream>
#include <vector>
#include <queue>
#include <utility>
#include <memory>
#include "server_protocol.h"
#include "queue.h"
#include "client.h"

const int QUEUE_MAXSIZE = 10000;

Accepter::Accepter(Socket& socket) :
    protocol(), 
    skt(socket)
{
}

void Accepter::reap_dead() {
    client_handlers.remove_if([](const std::unique_ptr<Client>& c) {
        if (c->is_dead()) {
            c->join();
            return true;
        }
        return false;
    });
}

void Accepter::kill_all() {
    for (const auto& c : client_handlers) {
        c->kill();
        c->join();
    }
    client_handlers.clear();
}

void Accepter::run() {
    GameHandler game_handler;
    while (is_server_open) {
        try {
            Socket peer(skt.accept());
            auto client = std::make_unique<Client>(
                std::move(peer), protocol, game_handler);
            client->start();
            // Remuevo clientes desconectados
            reap_dead();
            client_handlers.push_back(std::move(client));
        } catch (const LibError& err) {
            if (std::string(err.what()).compare(
                "socket accept failedInvalid argument") == 0) {
                is_server_open = false;
                break;
            }
            throw;
        }
    }
    kill_all();
}
