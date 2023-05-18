#include "accepter.h"
#include <tuple>
#include <iostream>
#include <exception>
#include <chrono>
#include <thread>
#include <string>
#include <sstream>

int main(int argc, char *argv[]) { try {
    int ret = 1;

    const char *servname = NULL;

    if (argc == 2) {
        servname = argv[1];
    } else {
        std::cerr << "Bad program call. Expected "
                << argv[0]
                << " <puerto/servicename>\n";
        return ret;
    }
    Socket sk(servname); // socket aceptador
    Accepter accepter(std::ref(sk));
    accepter.start();
    while (std::cin.get() != 'q') {
    }
    sk.shutdown(0);
    sk.close();
    accepter.join();
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
