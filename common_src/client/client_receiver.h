#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <atomic>
#include "client_protocol.h"
#include "../queue.h"
#include "../thread.h"
#include "../socket.h"
#include "client_image.h"

class ClientReceiver:public Thread {
//thread que recibe imagenes de modelos a renderizar
private:
    CProtocol protocol;
    Socket& skt;
    Queue<ProtocolResponse>& q;
    std::atomic<bool> keep_talking{true};
    std::atomic<bool> is_alive{true};

public:
    explicit ClientReceiver(
        Socket& socket,
        Queue<ProtocolResponse>& q);
    ~ClientReceiver() override;
    void run() override;
    bool is_dead();
    void kill();
};
