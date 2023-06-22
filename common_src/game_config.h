#ifndef _GAME_CONFIG_H
#define _GAME_CONFIG_H
#include <map>
#include <mutex>
#include <vector>
#include <yaml-cpp/yaml.h>

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

#define RESOLUTION_X 1920

//game.h
#define GAME_TICK_RATE 5
#define ZOMBIE_CREATION_TIME_MIN 10000
#define ZOMBIE_CREATION_TIME_MAX 15000
#define SCORE_TO_WIN 10
#define MAX_ZOMBIES 4
#define MAX_PLAYERS 1
#define WAIT_TIME_START_GAME 10000

//playerstate.h
#define RESPAWN_TIME 100

//armas
#define ARMA1_DAMAGE 5
#define ARMA1_BULLET_COUNT 20
#define ARMA1_MAGAZINE 50
#define ARMA1_RELOAD_DELAY 40
#define ARMA1_SHOOT_DELAY 20
#define ARMA1_GRENADE_DELAY 10

#define ARMA2_DAMAGE 6
#define ARMA2_BULLET_COUNT 10
#define ARMA2_MAGAZINE 30
#define ARMA2_RELOAD_DELAY 10
#define ARMA2_SHOOT_DELAY 20
#define ARMA2_GRENADE_DELAY 3000

#define ARMA3_DAMAGE 150
#define ARMA3_BULLET_COUNT 1
#define ARMA3_MAGAZINE 10
#define ARMA3_RELOAD_DELAY 20
#define ARMA3_SHOOT_DELAY 10
#define ARMA3_GRENADE_DELAY 1000

//granadas

#define GRANADA_DAMAGE 95
#define GRANADA_DELAY_12 10 //tiempo en explotar
#define GRANADA_DELAY_23 0  //tiempo en aparecer humo
#define GRANADA_DELAY_3 15   //tiempo en desaparecer el humo
#define GRANADA_SIZE 128
#define GRANADA_FUERZA 30 //distancia por tick de tecla apretada


// common
#define DEFAULT_MAX_X 1920
#define DEFAULT_MAX_Y 145
#define STARTING_HIT_POINTS 100
#define EVENTS_PER_LOOP 20

//zombie calculate_next_movement code;
#define CODE_NULL 0
#define CODE_VENOM_PROJECTILE 3
#define CODE_WITCH_SPAWN 4

// Player
#define GUN_MAGAZINE_SIZE 10
#define PLAYER_RECT_WIDTH 20
#define PLAYER_RECT_HEIGHT 20

// Zombie
#define ZOMBIE_RECT_WIDTH 20
#define ZOMBIE_RECT_HEIGHT 20
#define ZOMBIE_SEEKING_DISTANCE 400
#define ZOMBIE_DAMAGE 5
#define ZOMBIE_SPEED 4
#define ZOMBIE_TIMER 15

// Spear
#define SPEAR_RECT_WIDTH 100
#define SPEAR_DAMAGE 7
#define SPEAR_HP 150
#define SPEAR_SPEED 3

// Venom
#define VENOM_RECT_WIDTH 96
#define VENOM_SEEKING_DISTANCE 1000
#define VENOM_HP 50
#define VENOM_SPEED 2
#define VENOM_PROJECTILE_COOLDOWN 20
#define VENOM_PROJECTILE_DAMAGE 50
#define VENOM_PROJECTILE_SPEED 10
#define VENOM_PROJECTILE_SIZE 64
#define VENOM_DAMAGE 3

//Witch
#define WITCH_SEEKING_DISTANCE 800
#define WITCH_HP 255

//Jumper
#define JUMPER_SEEKING_DISTANCE 300
#define JUMPER_DAMAGE 45
#define JUMPER_HP 50
#define JUMPER_SPEED 10
#define JUMPER_COOLDOWN 20

struct GameLoopSettings {
    int game_tick_rate;
    int zombie_creation_time_min;
    int zombie_creation_time_max;
    int score_to_win;
    int max_zombies;
    int max_players;
    int wait_time_start_game;

    GameLoopSettings(std::map<std::string, int>& values) {
        game_tick_rate = values["game_tick_rate"];
        zombie_creation_time_max = values["zombie_creation_time_max"];
        zombie_creation_time_min = values["zombie_creation_time_min"];
        score_to_win = values["score_to_win"];
        max_zombies = values["max_zombies"];
        max_players = values["max_players"];
        wait_time_start_game = values["wait_time_start_game"];
    }
};


struct GameConfig {
    private:
    static GameConfig* sInstance;
    static std::mutex mutex_;

    //constructor
    GameConfig();
    std::map<std::string, std::map<std::string, int>> parseMapNodes(
        const YAML::Node& config, const std::string& node_name);
    void parseConfigFile(const std::string& filename);

    //game.h

    // Member variables to store the parsed configuration data
    int port;
    int defaultMaxX;
    int defaultMaxY;
    int startingHitPoints;
    int eventsPerLoop;
    int gunMagazineSize;
    int playerRectWidth;
    int playerRectHeight;
    std::vector<std::pair<std::string, std::string>> mapPaths;
    std::map<std::string, std::map<std::string, int>> weapons;
    std::map<std::string, std::map<std::string, int>> enemies;
    std::map<std::string, std::map<std::string, int>> soldiers;
    std::map<std::string, int> mathCodes;
    std::map<std::string, int> gameloop_values;

    public:
    //singleton
    //se llama para instanciar al objeto, o en caso de ya estar construido, devuelve el puntero
    static GameConfig* get_instance();
    // static GameConfig& getInstance() {
    //     static GameConfig instance;
    //     return instance;
    // }

    // Member functions to access the parsed configuration data
    int getPort() const;
    int getDefaultMaxX() const;
    int getDefaultMaxY() const;
    int getStartingHitPoints() const;
    int getEventsPerLoop() const;
    int getGunMagazineSize() const;
    int getPlayerRectWidth() const;
    int getPlayerRectHeight() const;
    std::map<std::string, int> get_gameloop_values() const;
    std::vector<std::pair<std::string, std::string>> getMapPaths() const;
    std::map<std::string, std::map<std::string, int>> getEnemies() const;
    std::map<std::string, std::map<std::string, int>> getSoldiers() const;
    std::map<std::string, int> getMathCodes() const;
    std::map<std::string, int> get_weapon(const std::string& weapon_name);
    std::map<std::string, int> get_enemy(const std::string& enemy_name);
    //libera el objeto
    static void release();
};


#endif
