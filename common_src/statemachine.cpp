#include "statemachine.h"
#include "../server_src/player.h"
#include <iostream>
#include <exception>
#include <fstream>
#include <vector>
#include <cstring>
#include <tuple>

StateMachine::StateMachine()
{
    this->messages = {
        "Shooting? ",
        "Moving? ",
        "Reloading? ",
        "Position? ",
        "Rounds? "
    };
}

void StateMachine::print_game_state(const std::vector<int> &game_state)
{
    bool is_shooting = 
        game_state[0] == SHOOTING || game_state[0] == SHOOTINGANDMOVING ? 1 : 0;
    bool is_moving = 
        game_state[0] == MOVING || game_state[0] == SHOOTINGANDMOVING ? 1 : 0;
    bool is_reloading = game_state[0] == RELOADING ? 1 : 0;
    std::cout << "Shooting? " << is_shooting << std::endl;
    std::cout << "Moving? " << is_moving << std::endl;
    std::cout << "Reloading? " << is_reloading << std::endl;
    std::cout << "Position? " << game_state[1] << 
    " " << game_state[2] << std::endl;
    std::cout << "Rounds? " << game_state[3] << std::endl;
    std::cout << std::endl;
}
