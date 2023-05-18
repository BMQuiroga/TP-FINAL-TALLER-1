#include "client_reader.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include <algorithm>

ClientReader::ClientReader(Socket& socket) :
    protocol(), 
    skt(socket)
{
}

bool is_gone(const std::string &command) {
    return command.compare("leave") == 0;
}

void ClientReader::start() {
    std::string command;
    std::cin >> command;
    std::string parameter;
    ClientProtocol protocol;
    while (!is_gone(command)) {
        std::cin >> parameter;
        if (command.compare("read") == 0) {
            for (int i = 0; i < std::stoi(parameter); ++i) {
                std::string response = protocol.wait_response(
                    command, skt);
                print_message(command, response);
            }
            std::cin >> command;
            continue;
        }
        protocol.send_command(command, parameter, skt);
        if (command != "broadcast") {
            std::string response = protocol.wait_response(
                command, skt);
            print_message(command, response);
        }
        std::cin >> command;
    }
}

void ClientReader::print_message(
    const std::string &command, const std::string &result) {
    if (command == "create") {
        std::cout << "Created match: " << result << std::endl;
    } else if (command == "join") {
        if (std::stoi(result) == 1) {
            std::cout << "Match does not exist: " << result << std::endl;
        } else {
            std::cout << "Joined to match: " << result << std::endl;
        }
    } else if (command == "read") {
        std::cout << "Broadcasted: " << result << std::endl;
    }
}
