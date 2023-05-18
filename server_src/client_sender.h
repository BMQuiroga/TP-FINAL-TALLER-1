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
#include "../common_src/socket.h"
#include "queue.h"
#include "../common_src/thread.h"

#include "server_protocol.h"
#include "matchstate.h"

class ClientSender:public Thread {
private:
    Socket& skt;
    Queue<MatchState>& q;
    ServerProtocol& protocol;
    std::atomic<bool> keep_talking{true};
    std::atomic<bool> is_alive{true};

public:
    explicit ClientSender(Socket& socket,
        Queue<MatchState>& q,
        ServerProtocol& protocol);
    /**
     * Imprime el mensaje del comando recibido en pantalla
    */
    void print_message(const MatchState &state);
    void run() override;
    bool is_dead();
    void kill();
};
