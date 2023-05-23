#ifndef __PLAYER_STATE_H__
#define __PLAYER_STATE_H__

#include "protocol.h"
#include <vector>
#include <string>

#define GUN_MAGAZINE_SIZE 10
#define STARTING_HIT_POINTS 100
#define DEFAULT_MAX_X 100
#define DEFAULT_MAX_Y 100

enum player_state { IDLE, SHOOTING, MOVING, SHOOTING_AND_MOVING, RELOADING };

// Clase encargada de manejar la lógica del jugador (almacenar y actualizar su estado)
class PlayerState {
  private:
    std::string name;
    uint16_t hit_points;
    uint16_t rounds;
    std::vector<int8_t> direction;
    std::vector<uint16_t> position;
    int8_t state;
    int8_t max_x, max_y;
    void move();
    void shoot(int flag);

  public:
    PlayerState(int8_t max_x = DEFAULT_MAX_X, int8_t max_y = DEFAULT_MAX_Y);
    ~PlayerState();

    // Procesa la solicitud del cliente y actualiza el estado actual del jugador
    void next_state(const ProtocolRequest &request);
};
#endif