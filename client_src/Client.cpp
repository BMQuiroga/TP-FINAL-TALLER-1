#include "client.h"
#include "../common_src/statemachine.h"
#include <iostream>
#include <exception>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <tuple>

Client::Client(
        const std::string& hostname,
        const std::string& servname,
        const std::string& commands_file_name) :
    hostname(hostname),
    servname(servname),
    protocol(hostname, servname),
    file_handler(commands_file_name)
{
}

void Client::start_communication(){
    std::ifstream commands_file(this->file_handler.get_file_name());
    StateMachine statemachine;
    std::tuple command_info = file_handler.get_next_command(commands_file);
    while (!std::get<0>(command_info).empty()) {
        auto command = std::get<0>(command_info);
        std::vector<int> parameters = std::get<1>(command_info);
        std::vector<int> game_state = this->protocol.get(command, 
            parameters);
        statemachine.print_game_state(game_state);
        command_info = file_handler.get_next_command(commands_file);
    }
}
