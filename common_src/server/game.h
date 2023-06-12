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
#include <random>
#include <chrono>
#include <thread>
#include <cstdint>
#include <atomic>
#include <utility>
#include "../matchstate.h"
#include "../queue.h"
#include "../protocol_types.h"
#include "../thread.h"
#include "../player_state.h"
#include "../client/client_protocol.h"
#include "../protected_vector.h"
#include "../serialization.h"
#include "../bullet.h"
#include "../physics_manager.h"
#include "../zombie.h"
#include <ctime>

#define MAX_ZOMBIES 5
#define MAX_PLAYERS 2
#define FAILURE -1
#define GAME_TICK_RATE 15
#define ZOMBIE_CREATION_TIME_MIN 10000
#define ZOMBIE_CREATION_TIME_MAX 15000

using namespace std::placeholders;

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
    std::vector<PlayerState> players;
    std::list<Bullet> bullets;
    std::vector<CommonZombie> zombies;
    std::atomic<GameState> state;
    ProtectedVector<std::reference_wrapper<Queue<ProtocolResponse>>> message_queues;
    Serializer serializer;
    PhysicsManager *physics;
    // PropertyObserver<uint16_t, GameEntity> on_entity_moved;
  public:
    explicit GameLoop(
        Queue<GameEvent> &events) : events(events), state{CREATED} {
            physics = PhysicsManager::get_instance();
            physics->set_layer_collision_mask(
                CollisionLayer::Friendly,
                CollisionFlag::Hostile | CollisionFlag::HostileProjectile
            );
            physics->set_layer_collision_mask(
                CollisionLayer::FriendlyProjectile,
                CollisionFlag::Hostile
            );
            physics->set_layer_collision_mask(
                CollisionLayer::Hostile,
                CollisionFlag::Friendly | CollisionFlag::FriendlyProjectile
            );
            physics->set_layer_collision_mask(
                CollisionLayer::HostileProjectile,
                CollisionFlag::Friendly
            );
        }
        // on_entity_moved(std::bind(&GameLoop::_on_entity_moved, this, _1, _2, _3)) {}

    //une al player a la partida
    int join(GameEvent &event) {
        if (state == CREATED && players.size() < MAX_PLAYERS) {
            // PlayerState new_player(event.player_name);
            // new_player.x.attach(&on_entity_moved);
            players.push_back(PlayerState(event.player_name));
            // players.push_back(std::move(new_player));
            // physics->register_entity(&players.back(), CollisionLayer::Friendly);
            message_queues.push_back(*event.player_messages);
            return players.size();
        } else {
            return FAILURE;
        }
    }

    //crea un GSR con jugadores, zombies y eventos para enviar al cliente
    GameStateResponse make_response() {
        GameStateResponse resp;
        
        for (PlayerState &player : players) {
            resp.players.push_back(player.make_ref());
        }

        for (Bullet &b : bullets) {
            resp.players.push_back(b.make_ref());
        }

        for (CommonZombie &zombie : zombies) {
            resp.zombies.push_back(zombie.make_ref());
        }
        resp.game_state = state;
        return resp;
    }

    //devuelve al jugador con uuid
    PlayerState& get_player(const std::string &uuid) {
        for (PlayerState &player : players) {
            if (player.get_name() == uuid) {
                return player;
            }
        }
    }

    //actualiza las armas de los jugadores y mueve las balas
    void pass_time() {
        for (PlayerState &player : players) {
            player.pass_time();
        }
        for (auto it = bullets.begin(); it != bullets.end();) {
            it->move();
            if (it->is_off_scope()) {
                it = bullets.erase(it);
            } else {
                ++it;
            }
        }
    }

    void _on_entity_moved(GameEntity *entity, uint16_t old, uint16_t new_) {
        
    }

    //envia la GSR
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

    // Function to generate a random number within a given range
    int getRandomNumber(int min, int max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(min, max);
        return distrib(gen);
    }

    // Function to handle enemy spawns
    void spawn_enemy() {
        // Spawn an enemy
        int x = getRandomNumber(0, 800);  // Random X position within game area
        int y = getRandomNumber(0, 95);  // Random Y position within game area
        Vector2D position(x, y);
        CommonZombie common_zombie("zombie", position);
        zombies.push_back(std::move(common_zombie));
    }

    void run() override {
        int delayMilliseconds = static_cast<int>(1000.0 / GAME_TICK_RATE);
        auto game_started_time = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        while (state != ENDED) {
            int spawn_interval = getRandomNumber(ZOMBIE_CREATION_TIME_MIN, ZOMBIE_CREATION_TIME_MAX);
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
                    player.next_state(event.req.cmd,bullets);
                }
                push_response();
            } else {
                bool changed = false;
                for (PlayerState &player : players) {
                    if (player.get_state() != IDLE) {
                        player.next_state(-1,bullets);
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
            auto current_time = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
            auto interval = current_time - game_started_time;
            if (interval % spawn_interval < 100 && interval > 0 && zombies.size() < MAX_ZOMBIES) {
                spawn_enemy();
            }

            /*for (CommonZombie &zombie : zombies) {
                zombie.move()
            }*/
            if (!zombies.empty()) {
                physics->update();
            }
            

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
