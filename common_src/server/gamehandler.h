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

#define JOIN_SUCCESS 0
#define JOIN_FAILURE 1

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
    bool has(int code) {
        m.lock();
        Game searchGame(code, "find game");
        auto it = std::find(list.begin(), list.end(), searchGame);
        bool code_exists = it != list.end() ? true : false;
        m.unlock();
        return code_exists;
    }
    /**
     * Agrega una partida nueva a la lista
    */
    void add(Game &x) {
        m.lock();
        list.push_back(std::move(x));
        m.unlock();
    }

    /**
     * Recibe un número de código de partida y 
     * devuelve la partida con ese código
    */
    Game& get_game(int code) {
        m.lock();
        Game searchGame(code, "find game");
        auto it = std::find(list.begin(), list.end(), searchGame);
        m.unlock();
        return *it;
    }

    size_t get_game_count() {
        std::lock_guard<std::mutex> lock(m);
        return list.size();
    }

    std::vector<GameReference> get_refs() {
        std::vector<GameReference> refs;
        for (Game &game : list) {
            refs.push_back(game.make_ref());
        }

        return refs;
    }

    // void send_message_to_game(
    //     const MatchState &message,
    //     int code) {
    //     m.lock();
    //     auto it = std::find(list.begin(), list.end(), code);
    //     (*it).notify_all(message);
    //     m.unlock();
    // }

    /**
     * Cambia los jugadores de una partida según el 
     * código de la partida
    */
    // void change_game_players(int code, Queue<MatchState>& q) {
    //     m.lock();
    //     auto it = std::find(list.begin(), list.end(), code);
    //     (*it).add_player(q);
    //     m.unlock();
    // }
};

class GameHandler {
    private:
    ProtectedMatchCounter number_matches;
    ProtectedMatchesList matches;
    
    public:
        explicit GameHandler(int number_matches = 0);
        /**
         * Crea una nueva partida y devuelve el estado de
         * la partida con el código de la partida y el 
         * comando recibido
        */
        Game& create_new_game(const std::string& name,
        Queue<ProtocolResponse>& q);
        Game& get_game(int code);
        size_t get_game_count();
        ProtectedMatchesList& get_games();
        // /**
        //  * Recibe un mensaje y el código de la partida y
        //  * reenvie el mensaje a la partida con el código 
        //  * recibido 
        // */
        // void broadcast_message(
        //     const std::string &message, 
        //     const int &game_code);
        /**
         * Recibo la cola de un jugador y el código de una partida. 
         * Si la partida existe, agrega el jugador a la partida. 
         * En caso contrario, envía un mensaje que la partida
         * no existe.
        */
        int join_game(int code, std::string &player_name, 
        Queue<ProtocolResponse> *q);
        // /**
        //  * Recibo una de los comandos (join, create) para iniciar 
        //  * una partida (handshake),
        //  * Devuelve si el del comando comando, si fue ejecutado 
        //  * con exito o no y el código de la partida
        // */
        // MatchState start_game_chat(const std::string& command, 
        // const std::string& parameters,
        // Queue<MatchState>& q);
};
#endif
