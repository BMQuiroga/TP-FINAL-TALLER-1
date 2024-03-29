#ifndef _ACCEPTER_H
#define _ACCEPTER_H

#include <tuple>
#include <iostream>
#include <exception>
#include <thread>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <list>
#include "server_protocol.h"
#include "client.h"
#include "../thread.h"
#include "../socket.h"
#include "../liberror.h"

class Accepter:public Thread {
private:
    ServerProtocol protocol;
    Socket& skt;
    std::list<std::unique_ptr<Client>> client_handlers;
    bool is_server_open{true};

public:
    explicit Accepter(
        Socket& socket);
    /**
     * Libera a los clientes muertos
    */
    void reap_dead();
    /**
     * Desconecta a todos los clientes aun vivos
    */
    void kill_all();
    void run() override;
};

#endif
