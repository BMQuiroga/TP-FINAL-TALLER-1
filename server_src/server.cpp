#include "server.h"
#include "../common_src/statemachine.h"
#include <iostream>
#include <exception>
#include <fstream>
#include <vector>
#include <cstring>

Server::Server(
        const std::string& servname,
        int x, 
        int y) :
    servname(servname),
    protocol(servname),
    game(x,y)
{
}

int Server::start_communication() {
    bool was_closed = false; 
    char command[7] = {0};
    StateMachine statemachine;
    while (not was_closed) {
        /* commandReceived es un tuple que tiene en el 
         * primer valor el tipo de comando, SHOOT, MOVE, NOP, o RELOAD,
         * y en el segundo valor tiene los argumentos según el tipo
         * de comando
         */
        std::tuple commandReceived = this->protocol.receive(
            command, was_closed);

        if (std::get<0>(commandReceived).length() == 0) {
            was_closed = true;
        }
        if (was_closed)
            break;
        this->game.do_command(
            std::get<0>(commandReceived), 
            std::get<1>(commandReceived));
        std::vector<int> game_state = game.get_current_game_state();
        statemachine.print_game_state(game_state);
        this->protocol.send(game_state);
    }
    return 0;
}
