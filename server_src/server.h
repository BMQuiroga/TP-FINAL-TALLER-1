#include "server_protocol.h"
#include "game.h"
#include <tuple>
#include <iostream>
#include <exception>
#include <thread>
#include <string>
#include <sstream>
#include <vector>

class Server {
private:
    const std::string servname;
    ServerProtocol protocol;
    Game game;

public:
    explicit Server(
        const std::string& servname,
        int x, int y);
    int start_communication();
};
