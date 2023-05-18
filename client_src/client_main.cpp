//#include "client.h"
#include "client_protocol.h"
#include "../common_src/socket.h"
#include "client_reader.h"
#include <iostream>
#include <exception>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>

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
    ret = 0;
    ClientReader client(std::ref(skt));
    client.start();
    skt.shutdown(0);
    skt.close();
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
