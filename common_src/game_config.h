#ifndef _GAME_CONFIG_H
#define _GAME_CONFIG_H

/**
 * TODO: Implement a singleton that holds all configuration values,
 * and reads them from a YAML file upon initialization
 */

// common
#define DEFAULT_MAX_X 1920
#define DEFAULT_MAX_Y 95
#define STARTING_HIT_POINTS 100
#define EVENTS_PER_LOOP 20

// Player
#define GUN_MAGAZINE_SIZE 10
#define PLAYER_RECT_WIDTH 20
#define PLAYER_RECT_HEIGHT 20

// Zombie
#define ZOMBIE_RECT_WIDTH 20
#define ZOMBIE_RECT_HEIGHT 20

#endif