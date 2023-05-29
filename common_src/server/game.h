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
#include "../protocol.h"
#include "../thread.h"
#include "../player_state.h"
#include "../client/client_protocol.h"
#include "../protected_vector.h"

#define MAX_PLAYERS 2
#define FAILURE -1

enum GameState {
    CREATED, STARTED, ENDED
};

struct GameEvent {
    ProtocolRequest req;
    std::string player_uuid;
    Queue<ProtocolResponse> &player_messages;
    GameEvent(ProtocolRequest &req, std::string &uuid, Queue<ProtocolResponse> &q) : req(req), player_uuid(uuid), player_messages(q) {};
};

class GameLoop : public Thread {

  private:
    Queue<GameEvent> &events;
    uint32_t map[100][100];
    ProtectedVector<PlayerState> players;
    std::atomic<GameState> state;
    ProtectedVector<std::reference_wrapper<Queue<ProtocolResponse>>> message_queues;

  public:
    GameLoop(
        Queue<GameEvent> &events
    ) : events(events), map{0} {
        state = CREATED;
    }

    int join(GameEvent &event) {
        return players.call<int>([this, event](std::vector<PlayerState> &players) -> int {
            if (state == CREATED && players.size() < MAX_PLAYERS) {
                PlayerState new_player(event.player_uuid);
                players.push_back(new_player);
                message_queues.push_back(event.player_messages);
                return players.size();
            } else {
                return FAILURE;
            }
        });
    }

    ProtocolResponse make_response() {
        ProtocolResponse resp = players.call<ProtocolResponse>([this](std::vector<PlayerState> &players) {
            ProtocolResponse resp;
            for (PlayerState &player : players) {
                resp.players.push_back(player.make_ref());
            }
            return resp;
        });
        resp.game_state = state;
        return resp;
    }

    PlayerState& get_player(std::string &uuid) {
        return players.call<PlayerState&>([this, uuid](std::vector<PlayerState> &players) -> PlayerState& {
            for (PlayerState &player : players) {
                if (player.get_name() == uuid) {
                    return player;
                }
            }
        });
    }

    void run() override {
        while (state != ENDED) {
            GameEvent event = events.pop();
            std::cout << "Popped event" << std::endl;
            if (event.req.cmd == JOIN) {
                int code = join(event);
                if (code != FAILURE && players.size() == MAX_PLAYERS) {
                    state = STARTED;
                }
            } else {
                PlayerState &player = get_player(event.player_uuid);
                player.next_state(event.req.cmd);
                ProtocolResponse resp = make_response();
                std::for_each(message_queues.begin(), message_queues.end(),
                    [resp](Queue<ProtocolResponse> & queue) {
                        queue.push(resp);
                    }
                );
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
            Queue<ProtocolResponse> &player_messages
        );
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
