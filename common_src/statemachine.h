#include <iostream>
#include <exception>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <algorithm>

class StateMachine {
private:
    std::vector<std::string> messages;
public:
    StateMachine();
    /**
     * Recibe el estado del juego y lo imprime en pantalla
    */
    void print_game_state(const std::vector<int>& game_state);
};
