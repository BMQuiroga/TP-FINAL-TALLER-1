#include <iostream>
#include <string>
#include <sstream>
#include <atomic>
#include "client_protocol.h"
#include "../thread.h"
#include "../socket.h"
#include "../queue.h"

class ClientSender:public Thread {
private:
    //recibe comandos en una queue, los popea y envia al servidor
    CProtocol protocol;
    Socket& skt;
    Queue<Intention*>& q;
    std::atomic<bool> keep_talking{true};
    std::atomic<bool> is_alive{true};

public:
    explicit ClientSender(
        Socket& socket,
        Queue<Intention*>& q);
    void run() override;
    bool is_dead();
    void kill();
};
