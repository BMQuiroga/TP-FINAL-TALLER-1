#ifndef __PLAYER_STATE_H__
#define __PLAYER_STATE_H__

#include <vector>
#include <string>
#include <list>
#include "game_entity.h"
#include "protocol_types.h"
#include "armas.h"
#include "bullet.h"
#include "granadas.h"
#include "game_config.h"

class Arma;

class Grenade;

class Bullet;
// Clase encargada de manejar la lógica del jugador
//  (almacenar y actualizar su estado)
class PlayerState : public GameEntity {
  private:
    uint8_t hit_points;
    int8_t respawn_time;
    Arma* arma;
    int grenade_type;
    void move();
    void attack(GameEntity *other) override;

  public:
    explicit PlayerState(
      const std::string &name,
      int id,
      int weapon_code = 3,
      PhysicsManager *physics = nullptr,
      int16_t max_x = GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_X"),
      int16_t max_y = GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_Y"));
    PlayerState(PlayerState&&);
    ~PlayerState();

    // Procesa la solicitud del cliente y actualiza el estado actual del jugador
    void next_state(uint8_t cmd, std::list<Bullet>& vec, uint32_t& bullets, std::list<Grenade>& gren, bool& sr);
    void take_damage(uint8_t damage) override;
    void pass_time();
    void on_collission_detected(GameEntity *other) override;
    bool is_dead();
    //uint8_t get_GameConfig::get_instance()->get_value<int>("RESPAWN_TIME")();
    //void try_respawn();

    std::string get_name();
    uint8_t get_hit_points();
    PlayerStateReference make_ref();

    //friend class Arma;
};
#endif
