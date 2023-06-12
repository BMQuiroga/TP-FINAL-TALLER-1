#include "bullet.h"
#include "protocol_types.h"
#include "physics_manager.h"

Bullet::Bullet(uint8_t dmg, uint8_t bc, entity_direction direc, bool piercing, Vector2D position) :
    GameEntity(
        "Im a bullet", 
        position,
        DEFAULT_MAX_X,
        DEFAULT_MAX_Y,
        CollisionLayer::FriendlyProjectile
    ) {
    this->damage = 25;
    this->piercing = piercing;
    this->facing_direction = direc;
    this->bullet_count = bc;
}

void Bullet::move() {
    if (this->facing_direction == LEFT) {
        position.x-=95;
    } else {
        position.x+=95;
    }
    //std::cout << "MOVED" << std::endl;
}

void Bullet::on_collission_detected(GameEntity *other) {

}

void Bullet::attack(GameEntity *other) {
    //????????????????
}

bool Bullet::is_off_scope() {
    return this->max_x < position.x;
}

PlayerStateReference Bullet::make_ref() {
    PlayerStateReference a;
    a.direction = facing_direction;
    a.x = position.x;
    a.y = position.y;
    a.name = "";
    a.hit_points = 0;
    a.id = 151;//importante para que sea un sonido
    //std::cout << "MAKE REF with id: " << a.id <<std::endl;
    return a;
}
