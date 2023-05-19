#include "client.h"

#include <iostream>
#include <exception>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>
#define FILE_ARG argv[3]


int main(int argc, char *argv[]) { try {
    int ret = 1;
    const char *hostname = NULL;
    const char *servname = NULL;

    if (argc == 4) {
        hostname = argv[1];
        servname = argv[2];
        std::ifstream text_file(FILE_ARG);
        if (!text_file) {
            std::cerr << "Bad program call. "
                  << FILE_ARG
                  << " is not a valid file.\n";
            return ret;
        }
    } else {
        std::cerr << "Bad program call. Expected "
                  << argv[0]
                  << " <ip/hostname server> <puerto/servicename> "
                  << "<archivo-acciones>.\n";
        return ret;
    }

    const char *commands_file_name = argv[3];

    Client client(hostname, servname, commands_file_name);
    client.start_communication();
    ret = 0;
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
