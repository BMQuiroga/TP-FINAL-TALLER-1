#ifndef __PLAYER_STATE_H__
#define __PLAYER_STATE_H__

#include <vector>
#include <string>
#include <list>
#include "game_entity.h"
#include "protocol_types.h"
#include "armas.h"
#include "bullet.h"
#include "game_config.h"

class Arma;

// Clase encargada de manejar la lógica del jugador
//  (almacenar y actualizar su estado)
class PlayerState : public GameEntity {
  private:
    uint8_t hit_points;
    int8_t respawn_time;
    Arma* arma;
    void move();
    void attack(GameEntity *other) override;

  public:
    explicit PlayerState(
      const std::string &name,
      int id,
      int16_t max_x = DEFAULT_MAX_X,
      int16_t max_y = DEFAULT_MAX_Y,
      int weapon_code = 1);
    PlayerState(PlayerState&&);
    ~PlayerState();

    // Procesa la solicitud del cliente y actualiza el estado actual del jugador
    void next_state(uint8_t cmd, std::list<Bullet>& vec);
    void take_damage(uint8_t damage);
    void pass_time();
    void on_collission_detected(GameEntity *other) override;
    bool is_dead();
    //uint8_t get_respawn_time();
    //void try_respawn();

    std::string get_name();
    PlayerStateReference make_ref();

    //friend class Arma;
};
#endif
