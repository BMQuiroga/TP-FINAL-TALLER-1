//#include "client.h"
#include <iostream>
#include <exception>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>
#include "../socket.h"
#include "../network/receiver.h"
#include "client_protocol.h"

int main(int argc, char *argv[]) { try {
    int ret = 1;
    const char *hostname = NULL;
    const char *servname = NULL;

    if (argc == 3) {
        hostname = argv[1];
        servname = argv[2];
    } else {
        std::cerr << "Bad program call. Expected "
                  << argv[0]
                  << " <ip/hostname server> <puerto/servicename>.\n";
        return ret;
    }
    
    Socket skt(hostname, servname);
    // TODO: Run GUI and start the client's receiver and sender threads
    return ret;
} catch (const std::exception& err) {
    std::cerr
        << "Something went wrong and an exception was caught: "
        << err.what()
        << "\n";
    return -1;
} catch (...) {
    std::cerr << "Something went wrong and an unknown exception was caught.\n";
    return -1;
} }
