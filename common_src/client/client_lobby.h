#include <iostream>
#include <string>
#include <sstream>
#include <atomic>
#include "client_protocol.h"
#include "../thread.h"
#include "../socket.h"
#include "../queue.h"

class ClientLobby:public Thread {
private:
    CProtocol protocol;
    Socket& skt;
    Queue<LobbyCommand>& q;
    Queue<LobbyGameStateResponse>& q_responses;
    std::atomic<bool> player_signed_up{false};
    std::atomic<bool> keep_talking{true};
    std::atomic<bool> is_alive{true};

public:
    explicit ClientLobby(
        Socket& socket,
        Queue<LobbyCommand>& q,
        Queue<LobbyGameStateResponse>& q_responses);
    ~ClientLobby() override;
    void run() override;
    bool is_dead();
    void kill();
};
