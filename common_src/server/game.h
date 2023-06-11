#ifndef _GAME_H
#define _GAME_H

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
#include <tuple>
#include <atomic>
#include "../matchstate.h"
#include "../queue.h"
#include "../protocol_types.h"
#include "../thread.h"
#include "../player_state.h"
#include "../client/client_protocol.h"
#include "../protected_vector.h"
#include "../serialization.h"

#define MAX_PLAYERS 2
#define FAILURE -1
#define GAME_TICK_RATE 15

enum GameState {
    CREATED, STARTED, ENDED
};

struct GameEvent {
    ProtocolRequest req;
    std::string player_name;
    Queue<ProtocolResponse> *player_messages;
    GameEvent() {}
    GameEvent(ProtocolRequest &req, std::string &uuid, Queue<ProtocolResponse> &q) : req(req), player_name(uuid), player_messages(&q) {}
    GameEvent& operator=(const GameEvent& other) {
        if (this == &other)
            return *this;
        req = other.req;
        player_name = other.player_name;
        player_messages = other.player_messages;

        return *this;
    }
};

class GameLoop : public Thread {
  private:
    Queue<GameEvent> &events;
    uint32_t map[100][100];
    std::vector<PlayerState> players;
    std::atomic<GameState> state;
    ProtectedVector<std::reference_wrapper<Queue<ProtocolResponse>>> message_queues;
    Serializer serializer;
  public:
    explicit GameLoop(
        Queue<GameEvent> &events) : events(events), map{0}, state{CREATED} {}
        // on_entity_moved(std::bind(&GameLoop::_on_entity_moved, this, _1, _2, _3)) {}

    int join(GameEvent &event) {
        if (state == CREATED && players.size() < MAX_PLAYERS) {
            PlayerState new_player(event.player_name);
            // new_player.x.attach(&on_entity_moved);  
            players.push_back(std::move(new_player));
            message_queues.push_back(*event.player_messages);
            return players.size();
        } else {
            return FAILURE;
        }
    }

    GameStateResponse make_response() {
        GameStateResponse resp;
        
        for (PlayerState &player : players) {
            resp.players.push_back(player.make_ref());
        }
        resp.game_state = state;
        return resp;
    }

    PlayerState& get_player(const std::string &uuid) {
        for (PlayerState &player : players) {
            if (player.get_name() == uuid) {
                return player;
            }
        }
    }

    void pass_time() {
        for (PlayerState &player : players) {
            player.pass_time();
        }
    }

    void _on_entity_moved(GameEntity *entity, uint16_t old, uint16_t new_) {
        std::cout << "Entity with uuid" << entity->get_name() << "moved!" << std::endl;
    }

    void push_response() {
        GameStateResponse resp = make_response();
        std::for_each(message_queues.begin(), message_queues.end(),
            [this, resp](Queue<ProtocolResponse> & queue) {
                ProtocolResponse response;
                response.content_type = GAME_STATE;
                response.content = serializer.serialize(resp);
                response.size = response.content.size();
                queue.push(response);
        });
    }

    void run() override {
        int delayMilliseconds = static_cast<int>(1000.0 / GAME_TICK_RATE);
        while (state != ENDED) {
            auto startTime = std::chrono::high_resolution_clock::now();
            GameEvent event;
            if (events.try_pop(event)) {
                // std::cout << "Popped event with cmd=" << std::to_string(event.req.cmd) << std::endl;
                if (event.req.cmd == JOIN) {
                    int code = join(event);
                    if (code != FAILURE && players.size() == MAX_PLAYERS) {
                        state = STARTED;
                    }
                } else {
                    PlayerState &player = get_player(event.player_name);
                    player.next_state(event.req.cmd);
                }
                push_response();
            } else {
                bool changed = false;
                for (PlayerState &player : players) {
                    if (player.get_state() != IDLE) {
                        player.next_state(-1);
                        changed = true;
                    }
                }
                // If any of the players has continued an action, broadcast it 
                // to the other players (example: movement)
                if (changed)
                    push_response();
            }
            
            pass_time();

            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

            // Calculate the remaining delay to reach the desired execution frequency
            int remainingDelayMilliseconds = delayMilliseconds - static_cast<int>(duration.count());

            if (remainingDelayMilliseconds > 0) {
                // Sleep for the remaining delay period
                std::this_thread::sleep_for(std::chrono::milliseconds(remainingDelayMilliseconds));
            }
        }
    }
};

class Game {
    private:
    int id;
    std::string name;
    std::list<std::reference_wrapper<Queue<ProtocolResponse>>> players;
    Queue<GameEvent> events;
    GameLoop loop;

    public:
        explicit Game(int id, const std::string& name);
        Game(Game&&);

        GameReference make_ref();

        void push_event(
            ProtocolRequest &req,
            std::string &player_name,
            Queue<ProtocolResponse> &player_messages);
        void start();

        /**
         * Agrego la cola de un jugador nuevo en la lista de colas
         * de jugadores
        */
        void add_player(Queue<ProtocolResponse>& q);
        /**
         * Pusheo un mensaje nuevo en las colas de
         * todos los juegos unidos en la partida
        */
        void notify_all(const ProtocolResponse& state);
        /**
         * Devuelve el id de una partida
        */
        int get_id() const;
        bool operator==(const Game &Game) const;
        bool operator==(const int &code) const;
};

#endif
