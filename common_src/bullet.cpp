#include "bullet.h"
#include "protocol_types.h"

Bullet::Bullet(uint8_t dmg, uint8_t bc, entity_direction direc, bool piercing, uint16_t x, uint16_t y) :
    GameEntity("Im a bullet",DEFAULT_MAX_X,DEFAULT_MAX_Y) {
    this->damage = 25;
    this->piercing = piercing;
    this->facing_direction = direc;
    this->bullet_count = bc;
    this->x = x;
    this->y = y;
}

void Bullet::move() {
    if (this->facing_direction == LEFT) {
        this->x-=95;
    } else {
        this->x+=95;
    }
    //std::cout << "MOVED" << std::endl;
}

void Bullet::attack() {
    //????????????????
}

bool Bullet::is_off_scope() {
    return this->max_x < this->x;
}

PlayerStateReference Bullet::make_ref() {
    PlayerStateReference a;
    a.direction = facing_direction;
    a.x = x;
    a.y = y;
    a.name = "";
    a.hit_points = 0;
    a.id = 151;//importante para que sea un sonido
    //std::cout << "MAKE REF with id: " << a.id <<std::endl;
    return a;
}