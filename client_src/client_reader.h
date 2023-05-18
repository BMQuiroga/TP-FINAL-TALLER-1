#include <iostream>
#include <string>
#include <sstream>
#include "client_protocol.h"
#include "../common_src/socket.h"

class ClientReader {
private:
    ClientProtocol protocol;
    Socket& skt;

public:
    explicit ClientReader(
        Socket& socket);
    void start();
    void print_message(
        const std::string &command, 
        const std::string &result);
};
