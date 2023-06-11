#ifndef _ZOMBIE_H
#define _ZOMBIE_H

#include "player_state.h"

enum attack_type {
  ZOMBIE_ATTACK1, 
  ZOMBIE_ATTACK2, 
  ZOMBIE_ATTACK3,
  ZOMBIE_JUMP,
  ZOMBIE_SCREAM,
  ZOMBIE_BITE,
};

enum movement_type {
  ZOMBIE_IDLE, ZOMBIE_RUN, ZOMBIE_WALK
};

enum ZOMBIE_TYPE {
  JUMPER, VENOM, SPEAR, WITCH, ZOMBIE
};

class Zombie : public GameEntity {
  protected:
    uint8_t damage;
    uint8_t zombie_type;
    uint8_t attack_type;
    uint8_t movement_type;
  public:
    Zombie(
        const std::string &name,
        int16_t max_x, 
        int16_t max_y
    );
    ~Zombie();
    void move();
};

class CommonZombie : public Zombie {
  public:
    CommonZombie(
        const std::string &name,
        int16_t max_x, 
        int16_t max_y
    );
};

#endif