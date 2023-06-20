#ifndef __GRANADAS_H__
#define __GRANADAS_H__

#include <iostream>
#include <list>
#include "player_state.h"
#include "zombie.h"

class Grenade : public GameEntity {
    private:
    int damage;
    int time_to_change;
    bool smoke;
    bool air_strike;
    bool dead;
    bool sound; //para q no repita el sonido del rebote

    public:
    //constructor
    Grenade(int type, uint16_t x,  uint16_t y, bool exploded_on_hand);

    //decrementa en 1 el delay
    void advance_time();

    PlayerStateReference make_ref();

    void on_collission_detected(GameEntity * other) override;

    void attack(GameEntity * other) override;

    bool is_dead();
};

#endif