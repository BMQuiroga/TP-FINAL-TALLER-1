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
#define RELOAD 10
#define PREPARE_GRENADE 11
#define THROW_GRENADE 12
#define IDLE -1

class Intention {
private:
    int id{-1};
public:
    virtual int get_intention() = 0;
    static Intention *make_intention(SDL_Event& event);
};

#endif
