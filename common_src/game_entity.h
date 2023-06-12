#ifndef _GAME_ENTITY_H
#define _GAME_ENTITY_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "math_helper.h"
#include "observable_property.h"
#include "physics_manager.h"

#define DEFAULT_MAX_X 1920
#define DEFAULT_MAX_Y 95

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
  EATING 
  };
enum entity_direction { LEFT, RIGHT };

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
    virtual void attack() = 0;

  public:
    GameEntity(const std::string &name, int16_t max_x, int16_t max_y, CollisionLayer layer);
    GameEntity(
      const std::string &name,
      Vector2D position,
      int16_t max_x, int16_t max_y,
      CollisionLayer layer
    );
    GameEntity(GameEntity&&);
    ~GameEntity();
    std::string get_name();
    int8_t get_state();
    uint32_t get_id();
    uint32_t get_physics_id();
    bool check_collision(GameEntity *other);
    virtual void on_collission_detected(GameEntity *other) = 0;
};

#endif
