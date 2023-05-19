#include <stdint.h>
#include <iostream>
#include <tuple>
#include <string>
#include <sstream>

enum PlayerState {
    IDLE,
    SHOOTING,
    MOVING, 
    SHOOTINGANDMOVING, 
    RELOADING
};

class Player {
    private:
    int coordenates_x {0};
    int coordenates_y {0};
    int moving_direction_x {0};
    int moving_direction_y {0};    
    PlayerState currentState {IDLE};

    public:
    Player();
    /**
     * Recibe las nuevas direcciones en x y en y, actualiza las direcciones
     * del jugador y cambia el estado del 
     * jugador según las direcciones recibidas
    */
    void change_direction(int x, int y);
    /**
     * El jugador se mueve según sus direcciones
    */
    void move();
    /**
     * Devuelve la ubicación actual del jugador en x y en y
    */
    std::tuple<int, int> get_location();
    /**
     * Devuelve la dirección actual del jugador en x y en y
    */
    std::tuple<int, int> get_direction();
    /**
     * Cambia el estado del jugador a SHOOTING o SHOOTINGANDMOVING
    */
    void start_shooting();
    /**
     * Cambia el estado del jugador a IDLE o MOVING
    */
    void stop_shooting();
    /**
     * Cambia el estado del jugador a IDLE o SHOOTING
    */
    void stop_moving();
    /**
     * Devuelve true si el estado del jugador es MOVING o SHOOTINGANDMOVING.
     * Devuelve false en caso contrario.
    */
    bool is_moving();
    /**
     * Devuelve true si el estado del jugador es SHOOTING o SHOOTINGANDMOVING.
     * Devuelve false en caso contrario.
    */
    bool is_shooting();
    /**
     * Devuelve el estado del jugador
    */
    PlayerState get_current_state();
    /**
     * Cambia el estado del jugador a IDLE
    */
    void reload();
};
