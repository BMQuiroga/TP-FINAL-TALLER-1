#include <stdint.h>
#include <iostream>
#include <string>
#include <sstream>
#include <tuple>
#include <vector>
#include "player.h"

class Game {
private:
    const int x_dimension;
    const int y_dimension;
    int rounds {10};
    Player player {Player()};
    
public:
    explicit Game(
        const int x_dimension,
        const int y_dimension);
    /**
     * Recibe un string con el comando y un vector con los parámetros
     * y ejecuta la acción requerida según el comando del juego
    */
    void do_command(
        const std::string& command, 
        const std::vector<int>& parameters);
    /**
     * Recibe la posición actual del jugador en x y la dirección en x y
     * verifica si la próxima posición es válida
    */
    bool verify_valid_location_x(int x, int player_location_x);
    /**
     * Recibe la posición actual del jugador en y y la dirección en y y
     * verifica si la próxima posición es válida
    */
    bool verify_valid_location_y(int y, int player_location_y);

    /**
     * Recibe la dirección en x y en y y verifica que la próxima posición en
     * x y en y sea válida
    */
    bool is_valid_location(int x, int y);
    /**
     * Recibe la dirección en x y en y y devuelve una dirección 
     * válida para moverse
    */
    std::tuple<int, int> get_valid_direction(int x, int y);
    /**
     * Devuelve el estado actual del juego
    */
    std::vector<int> get_current_game_state();

Game(const Game&) = delete;
Game& operator=(const Game&) = delete;
};
