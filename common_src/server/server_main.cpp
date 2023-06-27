#include "accepter.h"
#include <tuple>
#include <iostream>
#include <exception>
#include <chrono>
#include <thread>
#include <string>
#include <sstream>
#include "server.h"
#include "../game_config.h"

int main(int argc, char *argv[]) {
    int ret = 1;
    const char *config_path = NULL;

    if (argc == 2) {
        config_path = argv[1];
    } else {
        std::cerr << "Bad program call. Expected "
                << argv[0]
                << " <ruta/config_path>\n";
        return ret;
    }
    
    Server server(config_path);
    server.run();

    return 0;
}
