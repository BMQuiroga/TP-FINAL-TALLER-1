#ifndef _ZOMBIE_H
#define _ZOMBIE_H

#include "protocol_types.h"
#include "game_entity.h"
#include <memory>
#include <string>
#include <vector>

class PlayerState;

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
    uint8_t health{(uint8_t)GameConfig::get_instance()->get_value<int>("ZOMBIE_HP")};
    uint8_t zombie_type;
    uint8_t attack_type;
    uint8_t movement_type;
    uint8_t show_death_timer;
    int seeking_distance;
    uint8_t smoked_time{0};
    //bool has_target_set{false};
    //GameEntity *target;
    //Vector2D target_position{VEC2_ZERO};

  public:
    static Zombie* get_random_zombie(int secure, PhysicsManager *physics, std::vector<PlayerState>& players);
    static void generate_clear_the_area(int number_of_zombies, std::list<Zombie*>& list, PhysicsManager *physics, std::vector<PlayerState>& players);
    Zombie(
      const std::string &name,
      Vector2D position,
      int16_t max_x, 
      int16_t max_y,
      PhysicsManager *physics
    );
    Zombie(
      const std::string &name,
      Vector2D position,
      PhysicsManager *physics
    );
    ~Zombie();
    Zombie(Zombie&&);
    //de un zombie fabrica una GSR para enviar al cliente
    ZombieStateReference make_ref();
    void move();
    void set_id(int new_id);
    void set_direction(int x, int y);
    //void next_state();
    uint8_t get_damage();
    uint8_t get_health();
    void take_damage(uint8_t damage) override;
    bool try_dissapear();
    void process_smoke();
    void on_collission_detected(GameEntity *other) override;
    virtual int calculate_next_movement(std::vector<PlayerState>& players);
    virtual void attack(GameEntity *other) override;
};

class CommonZombie : public Zombie {
  public:
    CommonZombie(
      const std::string &name,
      Vector2D position,
      int16_t max_x = GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_X"),
      int16_t max_y = GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_Y"),
      PhysicsManager *physics = nullptr
    );
    CommonZombie(
      const std::string &name,
      Vector2D position,
      PhysicsManager *physics
    );
    ~CommonZombie();
    CommonZombie(CommonZombie&&);
    CommonZombie(const CommonZombie&) = default;  // Remove the 'delete'd declaration
};

class Jumper : public Zombie {
  private:
    int cooldown;
    Vector2D objetive;
  public:
    Jumper(
      const std::string &name,
      Vector2D position,
      int16_t max_x = GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_X"),
      int16_t max_y = GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_Y"),
      PhysicsManager *physics = nullptr
    );
    Jumper(
      const std::string &name,
      Vector2D position,
      PhysicsManager *physics
    );
    ~Jumper();
    Jumper(Jumper&&);
    Jumper(const Jumper&) = default;  // Remove the 'delete'd declaration
    void attack(GameEntity *other) override;
    int calculate_next_movement(std::vector<PlayerState>& players);
    void set_objetive(std::vector<PlayerState>& players);
    bool jump();
};

class Spear : public Zombie {
  public:
    Spear(
      const std::string &name,
      Vector2D position,
      int16_t max_x = GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_X"),
      int16_t max_y = GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_Y"),
      PhysicsManager *physics = nullptr
    );
    Spear(
      const std::string &name,
      Vector2D position,
      PhysicsManager *physics
    );
    ~Spear();
    Spear(Spear&&);
    Spear(const Spear&) = default;  // Remove the 'delete'd declaration
};

class Witch : public Zombie {
  public:
    Witch(
      const std::string &name,
      Vector2D position,
      int16_t max_x = GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_X"),
      int16_t max_y = GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_Y"),
      PhysicsManager *physics = nullptr
    );
    Witch(
      const std::string &name,
      Vector2D position,
      PhysicsManager *physics
    );
    ~Witch();
    Witch(Witch&&);
    Witch(const Witch&) = default;  // Remove the 'delete'd declaration
    void attack(GameEntity *other) override;
    int calculate_next_movement(std::vector<PlayerState>& players);
};

class Venom : public Zombie {
  private:
    int cooldown;
  public:
    Venom(
      const std::string &name,
      Vector2D position,
      int16_t max_x = GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_X"),
      int16_t max_y = GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_Y"),
      PhysicsManager *physics = nullptr
    );
    Venom(
      const std::string &name,
      Vector2D position,
      PhysicsManager *physics
    );
    ~Venom();
    Venom(Venom&&);
    Venom(const Venom&) = default;  // Remove the 'delete'd declaration
    int calculate_next_movement(std::vector<PlayerState>& players);
};

#endif
