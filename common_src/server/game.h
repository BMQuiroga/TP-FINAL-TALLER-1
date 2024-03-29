#ifndef _GAME_H
#define _GAME_H

#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <memory>
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
#include "../game_config.h"
#include "../granadas.h"
#include <ctime>

#define FAILURE -1

#define GM_SURVIVAL 2
#define GM_CTA 1

using namespace std::placeholders;

enum GameState {
    CREATED, STARTED, ENDED
};

struct GameEvent {
    ProtocolRequest req;
    std::string player_name;
    int weapon_code;
    Queue<ProtocolResponse> *player_messages;
    GameEvent() {}
    GameEvent(ProtocolRequest &req, std::string &uuid, 
    int weapon, Queue<ProtocolResponse> &q) : 
        req(req), 
        player_name(uuid), 
        weapon_code(weapon),
        player_messages(&q) {}
    GameEvent& operator=(const GameEvent& other) {
        if (this == &other)
            return *this;
        req = other.req;
        player_name = other.player_name;
        weapon_code = other.weapon_code;
        player_messages = other.player_messages;

        return *this;
    }
};

class GameLoop : public Thread {
  private:
    Queue<GameEvent> &events;
    uint8_t number_players;
    std::vector<PlayerState> players;
    std::list<Bullet> bullets;
    std::list<Zombie*> zombies;
    std::atomic<GameState> state;
    std::list<Grenade> grenades;
    std::list<Vomit_Projectile> vomit;
    ProtectedVector<std::reference_wrapper<Queue<ProtocolResponse>>> message_queues;
    Serializer serializer;
    PhysicsManager physics;
    uint32_t kills;
    uint32_t shots;
    uint32_t game_ticks;
    int game_mode;
    bool someone_reloaded;
    // PropertyObserver<uint16_t, GameEntity> on_entity_moved;
  public:
    explicit GameLoop(
        Queue<GameEvent> &events, uint8_t number_players=GameConfig::get_instance()->get_value<int>("MAX_PLAYERS"), int game_mode=GameConfig::get_instance()->get_value<int>("DEFAULT_MODE")) : 
        events(events), state{CREATED}, number_players(number_players), game_mode(game_mode) {
            physics.set_layer_collision_mask(
                CollisionLayer::Friendly,
                CollisionFlag::Hostile | CollisionFlag::HostileProjectile);
            physics.set_layer_collision_mask(
                CollisionLayer::FriendlyProjectile,
                CollisionFlag::Hostile);
            physics.set_layer_collision_mask(
                CollisionLayer::FriendlyExplosive,
                CollisionFlag::Friendly | CollisionFlag::Hostile);
            physics.set_layer_collision_mask(
                CollisionLayer::Hostile,
                CollisionFlag::Friendly | CollisionFlag::FriendlyExplosive);
            physics.set_layer_collision_mask(
                CollisionLayer::HostileProjectile,
                CollisionFlag::Friendly
            );
            kills = 0;
            shots = 0;
            game_ticks = 0;
            someone_reloaded = false;
        }
        // on_entity_moved(std::bind(&GameLoop::_on_entity_moved, this, _1, _2, _3)) {}

    void close() {
        if (state != ENDED) {
            state = ENDED;
            events.close();
            players.clear();
            bullets.clear();
            for (Zombie *z : zombies) {
                delete z;
            }
            zombies.clear();
            grenades.clear();
            vomit.clear();
            message_queues.clear();
            // physics->release();
        }
    }
    
    ~GameLoop() {
        close();
    }

    GameState get_state() {
        return state;
    }

    PlayerStateReference make_defeat() {
        PlayerStateReference a;
        a.id = 252;
        a.name = "defeat";
        a.state = kills;
        a.x = game_ticks;
        a.y = shots;
        return a;
    }

    PlayerStateReference make_victory() {
        PlayerStateReference a;
        a.id = 251;
        a.name = "victory";
        a.state = kills;
        a.x = game_ticks;
        a.y = shots;
        return a;
    }

    //une al player a la partida
    int join_game(GameEvent &event) {
        if (state == CREATED && players.size() < number_players) {
            players.push_back(PlayerState(event.player_name, players.size() + 1, event.weapon_code, &physics));
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
            if (b.get_state() == 0)
                resp.players.push_back(b.make_ref());
        }

        for (Grenade &b : grenades) {
            resp.players.push_back(b.make_ref());
        }

        for (Vomit_Projectile &b : vomit) {
            resp.players.push_back(b.make_ref());
        }

        if (someone_reloaded) {
            resp.players.push_back(Arma::make_reload());
            someone_reloaded = false;
        }

        for (Zombie* &zombie : zombies) {
            resp.zombies.push_back(zombie->make_ref());
        }
        resp.game_state = state;
        resp.kills = this->kills;
        resp.shots = this->shots;
        resp.time = this->game_ticks / GameConfig::get_instance()->get_value<int>("GAME_TICK_RATE");


        if (zombies.empty() && game_mode == GM_CTA) {//CTA termina cuando no hay mas zombies
            resp.players.push_back(make_victory());
            this->state = ENDED;
        } else if (is_everyone_dead()) {
            resp.players.push_back(make_defeat());
            this->state = ENDED;
        }
        return resp;
    }

    bool is_everyone_dead() {
        int i = 0;
        for (PlayerState &player : players) {
            if (player.is_dead())
                i++;
        }
        return i == players.size();
    }

    //devuelve al jugador con uuid
    PlayerState* get_player(const std::string &uuid) {
        for (PlayerState &player : players) {
            if (player.get_name() == uuid) {
                return &player;
            }
        }
        return nullptr;
    }

    void remove_player_from_game(Queue<ProtocolResponse> *q) {
        message_queues.erase(
            [q](Queue<ProtocolResponse>& queue) {
                return &queue == q;
            });
    }

    PlayerState& get_random_player() {
        int player_id = getRandomNumber(1, players.size());
        return players[player_id];
    }

    //actualiza las armas de los jugadores y mueve las balas
    void pass_time() {
        for (PlayerState &player : players) {
            player.pass_time();
        }
        for (auto it = bullets.begin(); it != bullets.end();) {
            it->move();
            if (it->is_dead()) {
                it = bullets.erase(it);
            } else {
                ++it;
            }
        }
        for (auto it = zombies.begin(); it != zombies.end();) {
            if ((*it)->try_dissapear()) {
                delete *it;
                it = zombies.erase(it);
                kills++;
            } else {
                ++it;
            }
        }
        for (auto it = grenades.begin(); it != grenades.end();) {
            it->advance_time();
            if (it->is_dead()) {
                it = grenades.erase(it);
            } else {
                ++it;
            }
        }
        for (auto it = vomit.begin(); it != vomit.end();) {
            it->move();
            if (it->is_dead()) {
                it = vomit.erase(it);
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
                // zombies.
        });
    }

    void push_game_lobby_state(int ready) {
        LobbyGameStateResponse resp;
        resp.ready = ready;
        resp.max_number_players = number_players;
        resp.number_players_connected = message_queues.size();
        resp.game_code;
        std::for_each(message_queues.begin(), message_queues.end(),
            [this, resp](Queue<ProtocolResponse> & queue) {
                ProtocolResponse response;
                response.content_type = GAME_START;
                response.content = serializer.serialize(resp);
                response.size = response.content.size();
                queue.push(response);
        });
    }

    // Function to handle enemy spawns
    void spawn_enemy(int secure) {
        std::cout << "spawn" << std::endl;
        zombies.push_back(Zombie::get_random_zombie(secure, &physics, players));
    }

    void run() override {
        while (state == CREATED) {
            GameEvent event;
            int n_events = 0;
            while (events.try_pop(event) && n_events < GameConfig::get_instance()->get_value<int>("EVENTS_PER_LOOP")) {
                n_events++;
                if (event.req.cmd == JOIN) {
                    int code = join_game(event);
                    if (code != FAILURE && players.size() == number_players) {
                        state = STARTED;
                        push_game_lobby_state(0);
                    } else {
                        push_game_lobby_state(1);
                    }
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(GameConfig::get_instance()->get_value<int>("WAIT_TIME_START_GAME") + GameConfig::get_instance()->get_value<int>("LAG_CONSTANT")));
        if (game_mode == GM_CTA)
            Zombie::generate_clear_the_area(GameConfig::get_instance()->get_value<int>("CTA_NUMBER_OF_ZOMBIES"),zombies, &physics, players);
        
        int delayMilliseconds = static_cast<int>(1000.0 / GameConfig::get_instance()->get_value<int>("GAME_TICK_RATE"));
        auto game_started_time = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        while (state == STARTED) {
            game_ticks++;
            int spawn_interval = getRandomNumber(GameConfig::get_instance()->get_value<int>("ZOMBIE_CREATION_TIME_MIN"), GameConfig::get_instance()->get_value<int>("ZOMBIE_CREATION_TIME_MAX"));
            auto startTime = std::chrono::high_resolution_clock::now();
            GameEvent event;
            int n_events = 0;
            while (events.try_pop(event) && n_events < GameConfig::get_instance()->get_value<int>("EVENTS_PER_LOOP")) {
                n_events++;
                // std::cout << "Popped event with cmd=" << std::to_string(event.req.cmd) << std::endl;
                PlayerState *player = get_player(event.player_name);
                if (event.req.cmd < 0) {
                    remove_player_from_game(event.player_messages);
                    if (message_queues.size() == 0) {
                        state = ENDED;
                        break;
                    }
                } else if (player) {
                    player->next_state(event.req.cmd,bullets,shots,grenades,someone_reloaded);
                }
            }
            for (PlayerState &player : players) {
                if (player.get_state() != IDLE) {
                    player.next_state(-1,bullets,shots,grenades,someone_reloaded); 
                }
            }
            int zombies_to_spawn_via_witch = 0;
            for (Zombie* &zombie : zombies) {
                int i = zombie->calculate_next_movement(players);
                if (i == GameConfig::get_instance()->get_value<int>("CODE_WITCH_SPAWN"))
                    zombies_to_spawn_via_witch++;
                if (i == GameConfig::get_instance()->get_value<int>("CODE_VENOM_PROJECTILE")) {
                    vomit.push_back(Vomit_Projectile(zombie->get_location(),zombie->get_facing_direction(), &physics));
                    //TODO crear el proyectil enemigo
                }           
            }
            //std::cout << "ZOMBIES SPAWNED VIA WITCH: " << zombies_to_spawn_via_witch << std::endl;
            for (int i = 0; i < zombies_to_spawn_via_witch ; i++) {
                spawn_enemy(5);//asegura que no sean brujas
            }
            
            pass_time();

            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            auto current_time = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
            auto interval = current_time - game_started_time;
            if (interval % spawn_interval < 100 && interval > 0 && zombies.size() < GameConfig::get_instance()->get_value<int>("MAX_ZOMBIES")) {
                if (game_mode == GM_SURVIVAL)
                    spawn_enemy(-1);//cualquier zombie
            }

            if (!zombies.empty()) {
                physics.update();
            }
            push_response();

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
    uint8_t number_players;
    uint8_t players_connected{0};
    int game_mode;
    std::list<std::reference_wrapper<Queue<ProtocolResponse>>> players;
    Queue<GameEvent> events;
    GameLoop loop;

    public:
        explicit Game(int id, const std::string& name);
        explicit Game(int id, const GameReference& game_ref);
        ~Game();
        Game(Game&&);
        Game& operator=(Game&&);

        GameReference make_ref();

        void push_event(
            ProtocolRequest &req,
            std::string &player_name,
            int weapon_code,
            Queue<ProtocolResponse> &player_messages);
        void start();
        /**
         * Pusheo un mensaje nuevo en las colas de
         * todos los juegos unidos en la partida
        */
        void notify_all(const ProtocolResponse& state);
        /**
         * Devuelve el id de una partida
        */
        int get_id() const;
        GameState get_state();
        bool operator==(const Game &Game) const;
        bool operator==(const int &code) const;
};

#endif
