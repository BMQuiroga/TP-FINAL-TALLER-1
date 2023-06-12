#ifndef _ZOMBIE_H
#define _ZOMBIE_H

#include "player_state.h"
#include "game_entity.h"
#include <string>

#define ZOMBIE_RECT_WIDTH 20
#define ZOMBIE_RECT_HEIGHT 20

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
  //clase zombie, hija de GameEntity
  protected:
    uint8_t damage;
    uint8_t health{STARTING_HIT_POINTS};
    uint8_t zombie_type;
    uint8_t attack_type;
    uint8_t movement_type;
  public:
    Zombie(
        const std::string &name,
        int16_t max_x, 
        int16_t max_y);
    ~Zombie();
    Zombie(Zombie&&);
    //de un zombie fabrica una GSR para enviar al cliente
    ZombieStateReference make_ref();
    void move();
    void set_id(int new_id);
    uint8_t get_damage();
    uint8_t get_health();
    void on_collission_detected(GameEntity *other) override;
};

class CommonZombie : public Zombie {
  public:
    CommonZombie(
        const std::string &name,
        Vector2D position,
        int16_t max_x = DEFAULT_MAX_X,
        int16_t max_y = DEFAULT_MAX_Y);
    ~CommonZombie();
    CommonZombie(CommonZombie&&);
    CommonZombie(const CommonZombie&) = default;  // Remove the 'delete'd declaration
    void attack() override;
};

#endif
