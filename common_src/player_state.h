#ifndef __PLAYER_STATE_H__
#define __PLAYER_STATE_H__

#include <vector>
#include <string>

#define GUN_MAGAZINE_SIZE 10
#define STARTING_HIT_POINTS 100
#define DEFAULT_MAX_X 100
#define DEFAULT_MAX_Y 100

enum player_state { IDLE, SHOOTING, MOVING, SHOOTING_AND_MOVING, RELOADING };
enum player_direction { LEFT, RIGHT };

// struct representing player attributes that will be visible to the clien
struct PlayerStateReference {
    uint8_t id;
    std::string name;
    uint8_t hit_points;
    uint8_t rounds;
    uint8_t direction;
    uint16_t x;
    uint16_t y;
    int8_t state;
};

// Clase encargada de manejar la lógica del jugador (almacenar y actualizar su estado)
class PlayerState {
  private:
    uint8_t id;
    std::string name;
    uint8_t hit_points;
    uint8_t rounds;
    std::vector<int8_t> direction;
    std::vector<uint16_t> position;
    int8_t state;
    int8_t max_x, max_y;
    void move();
    void shoot(int flag);

  public:
    explicit PlayerState(const std::string &name, int8_t max_x = DEFAULT_MAX_X, int8_t max_y = DEFAULT_MAX_Y);
    ~PlayerState();

    // Procesa la solicitud del cliente y actualiza el estado actual del jugador
    void next_state(int cmd);

    std::string get_name();
    PlayerStateReference make_ref();
};
#endif
