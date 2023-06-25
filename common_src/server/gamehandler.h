#ifndef _GAME_HANDLER_H
#define _GAME_HANDLER_H
#include <stdint.h>
#include <iostream>
#include "game.h"
#include <string>
#include <algorithm>
#include <vector>
#include <iterator>
#include <list>
#include <sstream>
#include <tuple>
#include <mutex>
#include <utility>
#include "../queue.h"
#include "../matchstate.h"
#include "../protocol.h"
#include "../protected_vector.h"

#define JOIN_SUCCESS 0
#define JOIN_FAILURE 1

class GameHandler {
    private:
    std::atomic<int> number_matches;
    ProtectedVector<Game> games;
    
    public:
        explicit GameHandler(int number_matches = 0);
        /**
         * Crea una nueva partida y devuelve el estado de
         * la partida con el código de la partida y el 
         * comando recibido
        */
        Game* create_new_game(const GameReference& game_ref,
        Queue<ProtocolResponse>& q);
        Game* get_game(int code);
        size_t get_game_count();
        ProtectedVector<Game>& get_games();
        std::vector<GameReference> get_refs();
        /**
         * Recibo la cola de un jugador y el código de una partida. 
         * Si la partida existe, agrega el jugador a la partida. 
         * En caso contrario, envía un mensaje que la partida
         * no existe.
        */
        int join_game(int code, std::string &player_name, 
        int weapon_code, Queue<ProtocolResponse> &q);
};
#endif
