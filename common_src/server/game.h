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

enum GameState {
    CREATED, STARTED, ENDED
};

struct GameEvent {
    ProtocolRequest req;
    std::string player_uuid;
    Queue<ProtocolResponse> *player_messages;
    GameEvent() {}
    GameEvent(ProtocolRequest &req, std::string &uuid, Queue<ProtocolResponse> &q) : req(req), player_uuid(uuid), player_messages(&q) {}
    GameEvent& operator=(const GameEvent& other) {
        if (this == &other)
            return *this;
        req = other.req;
        player_uuid = other.player_uuid;
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
    GameLoop(
        Queue<GameEvent> &events) : events(events), map{0} {
        state = CREATED;
    }

    int join(GameEvent &event) {
        if (state == CREATED && players.size() < MAX_PLAYERS) {
            PlayerState new_player(event.player_uuid);
            players.push_back(new_player);
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

    void run() override {
        int delayMilliseconds = static_cast<int>(1000.0 / 30);
        while (state != ENDED) {
            auto startTime = std::chrono::high_resolution_clock::now();
            GameEvent event = events.pop();
            std::cout << "Popped event with cmd=" << std::to_string(event.req.cmd) << std::endl;
            if (event.req.cmd == JOIN) {
                int code = join(event);
                if (code != FAILURE && players.size() == MAX_PLAYERS) {
                    state = STARTED;
                }
            } else {
                PlayerState &player = get_player(event.player_uuid);
                player.next_state(event.req.cmd);
            }
            GameStateResponse resp = make_response();
            std::for_each(message_queues.begin(), message_queues.end(),
                [this, resp, event](Queue<ProtocolResponse> & queue) {
                    ProtocolResponse response;
                    response.request = event.req;
                    response.content_type = GAME_STATE;
                    response.content = serializer.serialize(resp);
                    response.size = response.content.size();
                    queue.push(response);
            });
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

        void push_event(
            ProtocolRequest &req,
            std::string &player_uuid,
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
