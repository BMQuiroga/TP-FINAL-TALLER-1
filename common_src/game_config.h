#ifndef _GAME_CONFIG_H
#define _GAME_CONFIG_H

/**
 * TODO: Implement a singleton that holds all configuration values,
 * and reads them from a YAML file upon initialization
 */


//renderer
#define HP_BAR_LENGTH 50
#define HP_BAR_HEIGHT 3
#define Y_OFFSET 505
#define GAME_FRAME_RATE 30
#define MIN_MAX_VOLUME 128
#define GAME_VOLUME 10

#define LAG_CONSTANT 4000

#define RESOLUTION_X 1920

//game.h
#define GAME_TICK_RATE 10
#define ZOMBIE_CREATION_TIME_MIN 10000
#define ZOMBIE_CREATION_TIME_MAX 15000

#define MAX_ZOMBIES 4
#define MAX_PLAYERS 1
#define DEFAULT_MODE 1
#define WAIT_TIME_START_GAME 10000
#define CTA_NUMBER_OF_ZOMBIES 2

//playerstate.h
#define RESPAWN_TIME 100

//armas
#define BULLET_SPEED 95
#define BULLET_HITBOX 10

#define ARMA1_DAMAGE 20
#define ARMA1_BULLET_COUNT 20
#define ARMA1_MAGAZINE 50
#define ARMA1_RELOAD_DELAY 40
#define ARMA1_SHOOT_DELAY 20

#define ARMA1_GRENADE_DELAY 200 //HE

#define ARMA2_DAMAGE 6
#define ARMA2_BULLET_COUNT 10
#define ARMA2_MAGAZINE 30
#define ARMA2_RELOAD_DELAY 10
#define ARMA2_SHOOT_DELAY 20

#define ARMA2_GRENADE_DELAY 500 //BOMBARDER

#define ARMA3_DAMAGE 150
#define ARMA3_BULLET_COUNT 1
#define ARMA3_MAGAZINE 10
#define ARMA3_RELOAD_DELAY 20
#define ARMA3_SHOOT_DELAY 10

#define ARMA3_GRENADE_DELAY 200 //SMOKE

//granadas

#define GRANADA_DAMAGE 95
#define AIR_STRIKE_DAMAGE 45
#define SMOKE_DAMAGE 0

#define GRANADA_DELAY_12 10 //tiempo en explotar
#define GRANADA_DELAY_23 0  //tiempo en aparecer humo
#define GRANADA_DELAY_3 5   //tiempo en desaparecer el humo
#define GRANADA_SIZE 128
#define GRANADA_FUERZA 30 //distancia por tick de tecla apretada
#define BOMBARDER_SPACE_IN_BETWEEN 250 //distancia entre boombas del bombardero

// common
#define DEFAULT_MAX_X 3000
#define DEFAULT_MAX_Y 145
#define EVENTS_PER_LOOP 20

//zombie spawn
#define SPAWNER_SAFE_AREA_X 250

//zombie calculate_next_movement code;
#define CODE_NULL 0
#define CODE_VENOM_PROJECTILE 3
#define CODE_WITCH_SPAWN 4

// Player
#define PLAYER_RECT_WIDTH 20
#define PLAYER_RECT_HEIGHT 96
#define PLAYER_SPEED 10
#define PLAYER_HP 100

// Zombie
#define ZOMBIE_HP 100
#define ZOMBIE_TIMER 15

#include <iostream>
#include <fstream>
#include <yaml-cpp/yaml.h>

class GameConfig {
public:
    static GameConfig* get_instance(const char *config_path = nullptr);
    static void release();

    template <typename T>
    T get_value(const std::string& key) const {
        return config_[key].as<T>();
    }
    template <typename T>
    void set_value(const std::string& key, const T& value) {
        config_[key] = value;
    }
    void save();

private:
    static GameConfig *instance;
    GameConfig(const char *config_path);
    ~GameConfig();

    GameConfig(const GameConfig&) = delete;
    GameConfig& operator=(const GameConfig&) = delete;

    void load();

    YAML::Node config_;
    std::string path;
};


#endif
