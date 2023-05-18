#include <iostream>
#include <exception>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <tuple>
#include <stdint.h>
#include <string>
#include <sstream>
#include <cstdint>
#include <cstdlib>
#include <atomic>
#include "gamehandler.h"
#include "../common_src/socket.h"
#include "queue.h"
#include "../common_src/thread.h"
#include "server_protocol.h"

class ClientReceiver:public Thread {
private:
    Socket& skt;
    Queue<MatchState>& q;
    ServerProtocol& protocol;
    int game_code;
    GameHandler& game_handler;
    std::atomic<bool> keep_talking;
    std::atomic<bool> is_alive;

public:
    explicit ClientReceiver(Socket& socket,
        Queue<MatchState>& q,
        ServerProtocol& protocol,
        GameHandler &game_handler);

    void run() override;
    bool is_dead();
    void kill();
    /**
     * Envía el comando a ejecutar al manejador de partidas
    */
    void send_command(
        const std::string &command_name, 
        const std::string &command_args);
};
