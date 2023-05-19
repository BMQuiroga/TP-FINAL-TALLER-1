#include "game.h"

#include <stdint.h>
#include <tuple>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

Game::Game(
        const int x_dimension,
        const int y_dimension) : 
    x_dimension(x_dimension),
    y_dimension(y_dimension)
{
}

bool Game::verify_valid_location_x(int x, int player_location_x) {
    if (player_location_x + x >= this->x_dimension) {
        return false;
    }
    if (player_location_x + x < 0) {
        return false;
    }
    return true;
}

bool Game::verify_valid_location_y(int y, int player_location_y) {
    if (player_location_y + y >= this->y_dimension) {
        return false;
    }
    if (player_location_y + y < 0) {
        return false;
    }
    return true;
}

/* Recibe x e y, que representan si 
 * el jugador se mueve y en qué dirección */
std::tuple<int, int> Game::get_valid_direction(int x, int y) {
    std::tuple current_position = this->player.get_location();
    bool x_is_valid = verify_valid_location_x(x, std::get<0>(current_position));
    bool y_is_valid = verify_valid_location_y(y, std::get<1>(current_position));
    if (x_is_valid && y_is_valid) {
        return std::make_tuple(x, y);
    }
    if (x_is_valid) {
        return std::make_tuple(x, 0);
    }
    if (y_is_valid) {
        return std::make_tuple(0, y);
    }
    return std::make_tuple(0, 0);
}

/* Recibe x e y, que representan si 
 * el jugador se mueve y en qué dirección */
bool Game::is_valid_location(int x, int y) {
    std::tuple current_position = this->player.get_location();
    bool x_is_valid = verify_valid_location_x(x, std::get<0>(current_position));
    bool y_is_valid = verify_valid_location_y(y, std::get<1>(current_position));
    return x_is_valid && y_is_valid;
}

std::vector<int> Game::get_current_game_state() {
    if (this->player.is_shooting()) {
        this->rounds--;        
    }
    // Acá cambiar para que sean sí o sí big endian
    int player_state = (int) this->player.get_current_state();
    std::tuple current_position = this->player.get_location();
    int rounds_left = this->rounds;
    std::vector<int> current_state {
        player_state,
        std::get<0>(current_position),
        std::get<1>(current_position),
        rounds_left};
    if (this->rounds == 0) {
        this->player.stop_shooting();
    }
    return current_state;
}

void Game::do_command(const std::string& command, 
    const std::vector<int>& parameters){
    if (command == "MOVE") { 
        int x_location = parameters[0];
        int y_location = parameters[1];
        if (this->is_valid_location(x_location, y_location)){
            this->player.change_direction(x_location, y_location);
            this->player.move();
        } else {
            std::tuple valid_direction = 
            this->get_valid_direction(x_location, y_location);
            this->player.change_direction(
                std::get<0>(valid_direction), 
                std::get<1>(valid_direction));
            this->player.move();
            this->player.stop_moving();
        }
    } else if (command == "SHOOT") {
        if (this->rounds == 0) {
            return;
        }
        bool is_shooting = parameters[0];
        if (is_shooting) {
            this->player.start_shooting();
        } else {
            this->player.stop_shooting();
        }
    } else if (command == "RELOAD") {
        if (rounds < 10) {
            this->rounds = 10;
        }
        this->player.reload();
    } else if (command == "NOP") {
        if (this->player.is_moving()) {
            std::tuple current_direction = this->player.get_direction();
            int x = std::get<0>(current_direction);
            int y = std::get<1>(current_direction);
            if (!is_valid_location(x, y)) {
                std::tuple valid_direction = this->get_valid_direction(x, y);
                this->player.change_direction(
                    std::get<0>(valid_direction), 
                    std::get<1>(valid_direction));
                this->player.move();
                this->player.stop_moving();
            } else {
                this->player.move();
            }
        }
    }
}
