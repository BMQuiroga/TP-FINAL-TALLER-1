#include "zombie.h"

Zombie::Zombie(
    const std::string &name,
    int16_t max_x, 
    int16_t max_y
) : GameEntity(name, max_x, max_y) {}

Zombie::~Zombie() {}

void Zombie::move() {
    bool moved = GameEntity::move();
    if (moved) {
        if (state == IDLE) {
            state = MOVING;
        } else if (state == ATTACKING) {
            state = ATTACKING_AND_MOVING;
        }
    } else {
        if (state == ATTACKING_AND_MOVING) {
            state = ATTACKING;
        } else if (state == MOVING) {
            state = IDLE;
        }
    }
}

CommonZombie::CommonZombie(
    const std::string &name,
    int16_t max_x, 
    int16_t max_y
) : Zombie(name, max_x, max_y) {
    damage = 10;
    zombie_type = ZOMBIE;
    attack_type = ZOMBIE_BITE;
    movement_type = ZOMBIE_WALK;
}