#include <iostream>
#include "server.h"
#include "../game_config.h"

Server::Server(const char *config_path) : 
    skt(std::to_string(GameConfig::get_instance(config_path)->get_value<int>("SERVER_PORT")).c_str()),
    accepter(skt),
    closed(false) {}

Server::~Server() {
    close();
}

void Server::close() {
    if (!closed) {
        GameConfig::get_instance()->release();
        skt.shutdown(SHUT_RD);
        skt.close();
        accepter.join();
        closed = true;
    }
}

void Server::run() {
    try {
        accepter.start();
        while (std::cin.get() != 'q') continue;
        std::cout << "Closing server..." << std::endl;
    } catch (const std::exception& err) {
        std::cerr
            << "Something went wrong and an exception was caught: "
            << err.what()
            << "\n";
    } catch (...) {
        std::cerr << "Something went wrong and an unknown exception was caught.\n";
    }
    close();
}