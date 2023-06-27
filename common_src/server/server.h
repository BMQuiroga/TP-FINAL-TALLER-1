#ifndef _SERVER_H
#define _SERVER_H

#include "./accepter.h"

class Server {

  private:
    Socket skt;
    Accepter accepter;
    bool closed;
    void handle_sigint(int signum);

  public:
    explicit Server(const char *config_path);
    ~Server();
    void run();
    void close();
};

#endif
