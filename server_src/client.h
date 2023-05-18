#ifndef CLIENT
#define CLIENT
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
#include "../common_src/socket.h"
#include "queue.h"
#include "../common_src/thread.h"
#include <condition_variable>
#include "server_protocol.h"
#include <atomic>
#include "client_receiver.h"
#include "client_sender.h"

class Client {
private:
    Socket skt;
    Queue<MatchState> q;
    ClientReceiver th_r;
    ClientSender th_s;

public:
    explicit Client(Socket&& socket,
    ServerProtocol& protocol,
    GameHandler &game_handler);
    /**
     * Inicia los threads para recibir y enviar mensajes 
    */
    void start();
    void join();
    bool is_dead();
    void kill();

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;

    Client(Client&&) = default;
    Client& operator=(Client&&) = default;
};

#endif
