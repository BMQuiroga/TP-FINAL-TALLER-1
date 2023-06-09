#ifndef __PLAYER_STATE_H__
#define __PLAYER_STATE_H__

#include <vector>
#include <string>
#include "game_entity.h"
#include "protocol_types.h"
#include "armas.h"

#define GUN_MAGAZINE_SIZE 10
#define STARTING_HIT_POINTS 100
#define DEFAULT_MAX_X 1920
//#define DEFAULT_MAX_Y 900
#define DEFAULT_MAX_Y 95

class Arma;

// Clase encargada de manejar la lógica del jugador
//  (almacenar y actualizar su estado)
class PlayerState : public GameEntity {
  private:
    uint8_t hit_points;
    Arma * arma;
    void move();
    void attack() override;

  public:
    explicit PlayerState(
      const std::string &name,
      int16_t max_x = DEFAULT_MAX_X,
      int16_t max_y = DEFAULT_MAX_Y);
    ~PlayerState();

    // Procesa la solicitud del cliente y actualiza el estado actual del jugador
    void next_state(uint8_t cmd);

    void pass_time();

    std::string get_name();
    PlayerStateReference make_ref();

    //friend class Arma;
};
#endif
