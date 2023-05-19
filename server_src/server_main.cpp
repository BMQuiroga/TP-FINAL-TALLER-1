#include "server.h"
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
    // Declaro las variables para las dimensiones del mapa de juego
    int x;
    int y;

    if (argc == 4) {
        servname = argv[1];
        std::istringstream x_arg(argv[2]);
        std::istringstream y_arg(argv[3]);
        x_arg >> x;
        y_arg >> y;
    } else {
        std::cerr << "Bad program call. Expected "
                << argv[0]
                << " <puerto/servicename> <max-x> <max-y>\n";
        return ret;
    }

    Server server(servname, x, y);


    ret = server.start_communication();
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
