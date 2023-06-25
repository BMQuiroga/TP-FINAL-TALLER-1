#include "accepter.h"
#include <tuple>
#include <iostream>
#include <exception>
#include <chrono>
#include <thread>
#include <string>
#include <sstream>
#include "../game_config.h"

int main(int argc, char *argv[]) {
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
    try {
        accepter.start();
        while (std::cin.get() != 'q') continue;
        std::cout << "Closing server..." << std::endl;
        GameConfig::get_instance()->release();
        PhysicsManager::get_instance()->release();
        sk.shutdown(SHUT_RD);
        sk.close();
        accepter.join();
        ret = 0;
        return ret;
    } catch (const std::exception& err) {
        std::cerr
            << "Something went wrong and an exception was caught: "
            << err.what()
            << "\n";
        sk.shutdown(SHUT_RD);
        sk.close();
        accepter.join();
        return -1;
    } catch (...) {
        std::cerr << "Something went wrong and an unknown exception was caught.\n";
        sk.shutdown(SHUT_RD);
        sk.close();
        accepter.join();
        return -1;
    }
}
