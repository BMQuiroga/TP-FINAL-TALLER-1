#include "granadas.h"

Grenade::Grenade(int type, uint16_t x, uint16_t y, bool exploded_on_hand) :
GameEntity("nade",DEFAULT_MAX_X,DEFAULT_MAX_Y,CollisionLayer::FriendlyProjectile) {
    this->state = 1;
    if (exploded_on_hand)
        state++;
    this->dead = false;
    this->sound = true;
    if (type == HE_GRENADE) {
        this->damage = GRANADA_DAMAGE;
        this->smoke = false;
        this->air_strike = false;
        this->time_to_change = GRANADA_DELAY_12;
    } else if (type == SMOKE_GRENADE) {
        this->damage = 0;
        this->smoke = true;
        this->air_strike = false;
        this->time_to_change = GRANADA_DELAY_12;
    } else if (type == AIR_STRIKE) {
        this->damage = AIR_STRIKE_DAMAGE;
        this->smoke = false;
        this->air_strike = true;
        this->time_to_change = GRANADA_DELAY_12;
    }
    this->position = Vector2D(x,y);
    this->rect_width = GRANADA_SIZE;
    this->rect_height = GRANADA_SIZE;
}

void Grenade::advance_time() {
    std::cout << "TIME ADVANCED STATE: " << std::to_string(state) << " AND TIME TO CHANGE: " << time_to_change << std::endl;
    if (smoke)
        std::cout << "SMOKE ADVANCED STATE: " << std::to_string(state) << " AND TIME TO CHANGE: " << time_to_change << std::endl;
    this->time_to_change--;
    if (time_to_change <= 0) {         
        if (state == 1) {
            state = 2;
            time_to_change = GRANADA_DELAY_23;
        } else if (state == 2) {
            state = 3;
            time_to_change = GRANADA_DELAY_3;
        } else if (state == 3) {
            state = 4;
        } else if (state == 4) {
            dead = true;
        }
    }
}

bool Grenade::is_dead() {
    return dead;
}

PlayerStateReference Grenade::make_ref() {
    //si esta en state 1, manda el sonido de la granada rebotando
    //state 2: ruido de explosion
    //state 3: textura de explosion
    PlayerStateReference a;
    a.x = position.x;
    a.y = position.y;
    if (state == 1 && !air_strike && sound) {
        a.id = 157; //ruido de rebote
        sound = false;
    } else if (state == 1 && air_strike) {
        a.id = 162;
    } else if (state == 2 && !smoke) {
        a.id = 158; //ruido de HE
    } else if (state == 2 && smoke) {
        std::cout << "SMOKE NOISE" << std::endl;
        a.id = 159; //ruido de humo
    } else if (state == 3 && smoke) {
        a.id = 103; //humo gris
        std::cout << "SMOKE SMOKE" << std::endl;
    } else if (state == 3 && !smoke) {
        a.id = 102; //humo negro
    } else {
        a.id = 253;
    }
    a.state = DEAD;
    a.hit_points = 0;
    return a;
}

void Grenade::on_collission_detected(GameEntity * other) {
    if (state == 2)
        attack(other);
}

void Grenade::attack(GameEntity * other) {
    Zombie *z = (Zombie*)other;
    if (smoke)
        z->process_smoke();
    else
        z->take_damage(this->damage);
}