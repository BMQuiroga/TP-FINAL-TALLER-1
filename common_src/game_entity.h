#ifndef _GAME_ENTITY_H
#define _GAME_ENTITY_H

#include <iostream>
#include <vector>
#include <map>
#include <string>

// 0 idle, 1 attack, 2 dead, 3 grenade, 4 hurt 5 recharge, 6 shot, 7 walk, 
// 8 fall, 9 run, 10 protect, 11 run+atack, 12 bite, 13 scream, 14 eating
enum entity_state { 
  IDLE, 
  ATTACKING, 
  DEAD, 
  THROWING_GRENADE, 
  HURT, 
  RELOADING, 
  SHOT, 
  MOVING, 
  FALLING, 
  RUNNING, 
  BLOCKING, 
  ATTACKING_AND_MOVING, 
  BITING, 
  SCREAMING, 
  EATING 
  };
enum entity_direction { LEFT, RIGHT };

class GameEntity {
  protected:
    static int next_id; // Static member variable to track the next ID
    int id; // Instance-specific ID
    std::string name;
    uint16_t x;
    uint16_t y;
    std::vector<int8_t> direction;
    entity_direction facing_direction;
    uint16_t speed;
    int8_t state;
    int16_t max_x, max_y;
    bool move();
    virtual void attack() = 0;

  public:
    GameEntity(const std::string &name, int16_t max_x, int16_t max_y);
    GameEntity(GameEntity&&);
    ~GameEntity();
    std::string get_name();
    int8_t get_state();
};

#endif
