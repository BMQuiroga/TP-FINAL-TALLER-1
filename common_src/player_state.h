#ifndef __PLAYER_STATE_H__
#define __PLAYER_STATE_H__

#include <vector>
#include <string>
#include "protocol_types.h"

#define GUN_MAGAZINE_SIZE 10
#define STARTING_HIT_POINTS 100
#define DEFAULT_MAX_X 1820
#define DEFAULT_MAX_Y 900

// 0 idle, 1 attack, 2 dead, 3 grenade, 4 hurt 5 recharge, 6 shot, 7 walk, 8 fall, 9 run, 10 protect, 11 run+atack, 12 bite, 13 scream, 14 eating
enum player_state { 
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
enum player_direction { LEFT, RIGHT };

// Clase encargada de manejar la lógica del jugador (almacenar y actualizar su estado)
class PlayerState {
  private:
    uint8_t id;
    std::string name;
    uint8_t hit_points;
    uint8_t rounds;
    std::vector<int8_t> direction;
    uint8_t facing_direction;
    std::vector<uint16_t> position;
    uint16_t speed;
    int8_t state;
    int16_t max_x, max_y;
    void move();
    void shoot(int flag);

  public:
    explicit PlayerState(const std::string &name, int16_t max_x = DEFAULT_MAX_X, int16_t max_y = DEFAULT_MAX_Y);
    ~PlayerState();

    // Procesa la solicitud del cliente y actualiza el estado actual del jugador
    void next_state(int cmd);

    std::string get_name();
    PlayerStateReference make_ref();
};
#endif
