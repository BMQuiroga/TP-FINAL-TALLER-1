#ifndef INTENTION_H
#define INTENTION_H
#include <iostream>
#include <exception>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <algorithm>
#include <SDL2pp/SDL2pp.hh>
#include "../player_state.h"

#define MOVE_LEFT 0
#define MOVE_RIGHT 1
#define MOVE_UP 2
#define MOVE_DOWN 3
#define STOP_MOVING_LEFT 4
#define STOP_MOVING_RIGHT 5
#define STOP_MOVING_UP 6
#define STOP_MOVING_DOWN 7
#define SHOOT 8
#define STOP_SHOOTING 9
#define RELOAD 14
#define PREPARE_GRENADE 15
#define THROW_GRENADE 16
#define END -1

class Intention {
private:
    int id{-1};
public:
    //obtiene el codigo de la intencion para enviarla al servidor
    virtual int get_intention() = 0;

    //dado un evento, crea la intencion
    static Intention *make_intention(SDL_Event& event);
    ~Intention();
};

#endif
