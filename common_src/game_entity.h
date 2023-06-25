#ifndef _GAME_ENTITY_H
#define _GAME_ENTITY_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "math_helper.h"
#include "observable_property.h"
#include "physics_manager.h"
#include "game_config.h"

class PhysicsManager;
enum class CollisionLayer;

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
  EATING,
  JUMPING
  };
enum entity_direction { LEFT, RIGHT };

//clase virtual para entidad default
class GameEntity {
  protected:
    // static uint32_t next_id;
    uint32_t id; // Instance-specific ID
    uint32_t physics_id;
    std::string name;
    Vector2D position;
    uint16_t rect_width, rect_height;
    Vector2D direction;
    entity_direction facing_direction;
    uint16_t speed;
    int8_t state;
    int16_t max_x, max_y;
    CollisionLayer collision_layer;
    bool move();

  public:
    GameEntity(const std::string &name, int16_t max_x, int16_t max_y, CollisionLayer layer);
    GameEntity(
      const std::string &name,
      Vector2D position,
      int16_t max_x, int16_t max_y,
      CollisionLayer layer);
    GameEntity(GameEntity&&);
    ~GameEntity();
    std::string get_name();
    int8_t get_state();
    uint32_t get_id();
    uint32_t get_physics_id();
    Vector2D get_location();
    Vector2D get_direction();
    entity_direction get_facing_direction();
    bool check_collision(GameEntity *other);
    virtual void attack(GameEntity *other) = 0;
    virtual void on_collission_detected(GameEntity *other) = 0;
    virtual void take_damage(uint8_t dmg) {};
};

#endif
