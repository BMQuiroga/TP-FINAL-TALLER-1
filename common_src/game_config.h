#ifndef _GAME_CONFIG_H
#define _GAME_CONFIG_H

/**
 * TODO: Implement a singleton that holds all configuration values,
 * and reads them from a YAML file upon initialization
 */

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

// Spear
#define SPEAR_RECT_WIDTH 100
#define SPEAR_DAMAGE 7
#define SPEAR_HP 150
#define SPEAR_SPEED 3

// Venom
#define VENOM_RECT_WIDTH 128
#define VENOM_SEEKING_DISTANCE 1000
#define VENOM_HP 50
#define VENOM_SPEED 2
#define VENOM_PROJECTILE_COOLDOWN 20

//Witch
#define WITCH_SEEKING_DISTANCE 800
#define WITCH_HP 255

//Jumper
#define JUMPER_SEEKING_DISTANCE 200
#define JUMPER_DAMAGE 35
#define JUMPER_HP 35
#define JUMPER_SPEED 10
#define JUMPER_COOLDOWN 20

#endif