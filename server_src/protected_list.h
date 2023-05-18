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

class ProtectedMatchCounter {
    int counter {0};
    std::mutex m;
    public:
    explicit ProtectedMatchCounter(int x) : counter(x)
    {
    }
    /**
     * Incrementa el valor del contador
    */
    void inc() {
        m.lock();
        ++counter;
        m.unlock();
    }
    /**
     * Devuelve el valor del contador
    */
    int get_counter() {
        m.lock();
        int latest_count = counter;
        m.unlock();
        return latest_count;
    }
};

class ProtectedMatchesList {
    std::list<Game> list;
    std::mutex m;
    
    public:
    /**
     * Devuelve true si existe una partida con el código recibido
     * por parametro. 
     * Devuelve false en caso contrario.
    */
    bool has(int code);
    /**
     * Agrega una partida nueva a la lista
    */
    void add(const Game &x);
    /**
     * Recibe un número de código de partida y 
     * devuelve la partida con ese código
    */
    Game& get_game(int code);

    void send_message_to_game(
        const MatchState &message,
        int code);

    /**
     * Cambia los jugadores de una partida según el 
     * código de la partida
    */
    void change_game_players(int code, Queue<MatchState>& q);
};
