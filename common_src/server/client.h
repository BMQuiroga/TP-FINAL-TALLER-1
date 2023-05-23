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
#include <condition_variable>
#include <atomic>
#include "server_protocol.h"
#include "gamehandler.h"
#include "../socket.h"
#include "../queue.h"
#include "../thread.h"
#include "../protocol.h"
#include "../player_state.h"
#include "../network/receiver.h"
#include "../network/sender.h"

class Client {
private:
    Socket skt;
    Queue<ProtocolResponse> responses;
    Receiver<ProtocolResponse, ProtocolRequest> receiver;
    Sender<ProtocolResponse, ProtocolRequest> sender;
    PlayerState state;

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

    void handle_request(const ProtocolRequest &message);

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;

    Client(Client&&) = default;
    Client& operator=(Client&&) = default;
};

#endif
