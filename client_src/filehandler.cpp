#include "filehandler.h"

#include <stdint.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <tuple>

FileHandler::FileHandler(const std::string &commands_file_name, 
const std::string &command_line) : 
    commands_file_name(commands_file_name),
    command_line(command_line)
{
}

bool FileHandler::file_is_finished(std::ifstream &commands_file)
{
    return commands_file.eof();
}

bool FileHandler::is_valid_command()
{
    if (command_line[0] == '#') {
        return false;
    }
    command_line.erase(std::remove(command_line.begin(), 
    command_line.end(), '\n'), command_line.end());
    command_line.erase(std::remove(command_line.begin(), 
    command_line.end(), '\r'), command_line.end());
    if (command_line.empty()) {
        return false;
    }
    return true;
}

std::string FileHandler::get_file_name()
{
    return this->commands_file_name;
}

bool FileHandler::get_valid_command(std::ifstream &commands_file) 
{
    std::getline(commands_file, command_line);
    while (!this->is_valid_command()) {
        if (file_is_finished(commands_file)) {
            return false;
        }
        std::getline(commands_file, command_line);
    }
    return true;
}

std::tuple<std::string, std::vector<int>> FileHandler::get_next_command(
    std::ifstream &commands_file)
{
    if (!this->get_valid_command(commands_file)) {
        std::vector<int> args;
        return std::make_tuple("", args);
    }

    std::istringstream S(command_line); 
    std::string command;
    int first_parameter;
    int second_parameter;
    S >> command;
    S >> first_parameter;
    S >> second_parameter;
    std::vector parameters = {
        first_parameter,
        second_parameter
    };
    std::tuple <std::string, std::vector<int>> command_info = 
    std::make_tuple(command, parameters);
    return command_info;
}
