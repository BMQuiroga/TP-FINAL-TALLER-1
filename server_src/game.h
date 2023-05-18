#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <functional>
#include <list>
#include <map>
#include <algorithm>
#include <sstream>
#include "matchstate.h"
#include <tuple>
#include "queue.h"

class Game {
    private:
    int id;
    std::string name;
    std::list<std::reference_wrapper<Queue<MatchState>>> players;
    
    public:
        explicit Game(int id, const std::string& name);

        /**
         * Agrego la cola de un jugador nuevo en la lista de colas
         * de jugadores
        */
        void add_player(Queue<MatchState>& q);
        /**
         * Pusheo un mensaje nuevo en las colas de
         * todos los juegos unidos en la partida
        */
        void send_message(const MatchState& state);
        /**
         * Devuelve el id de una partida
        */
        int get_id() const;
        bool operator==(const Game &Game) const;
        bool operator==(const int &code) const;
};
