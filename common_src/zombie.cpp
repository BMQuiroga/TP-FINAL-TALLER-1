#include "zombie.h"
#include "physics_manager.h"
#include <string>
#include <utility>

Zombie::Zombie(
    const std::string &name,
    int16_t max_x, 
    int16_t max_y
) : GameEntity(
    name,
    max_x, max_y,
    CollisionLayer::Hostile) {
    rect_width = ZOMBIE_RECT_HEIGHT;
    rect_height = ZOMBIE_RECT_HEIGHT;
}

Zombie::~Zombie() {}

void Zombie::move() {
    bool moved = GameEntity::move();
    if (moved) {
        state = MOVING;
    } else {
        state = IDLE;
    }
}

void Zombie::on_collission_detected(GameEntity *other) {}

void Zombie::set_id(int new_id) {
    id = new_id;
}

uint8_t Zombie::get_damage()
{
    return damage;
}

uint8_t Zombie::get_health()
{
    return health;
}

Zombie::Zombie(Zombie&& other)
    : GameEntity(std::move(other)) {
    if (this == &other)
        return;
    damage = other.damage;
    zombie_type = other.zombie_type;
    attack_type = other.attack_type;
    movement_type = other.movement_type;
}

ZombieStateReference Zombie::make_ref()
{
    ZombieStateReference ref;
    ref.id = id;
    ref.damage = damage;
    ref.health = health;
    ref.zombie_type = zombie_type;
    ref.state = state;
    ref.attack_type = attack_type;
    ref.movement_type = movement_type;
    ref.x = (uint16_t)position.x;
    ref.y = (uint16_t)position.y;
    ref.direction = facing_direction;
    return ref;
}

CommonZombie::CommonZombie(
    const std::string &name,
    Vector2D position,
    int16_t max_x, 
    int16_t max_y
) : Zombie(name, max_x, max_y) {
    id = 51;
    damage = 10;
    position = position;
    zombie_type = ZOMBIE;
    attack_type = ZOMBIE_BITE;
    movement_type = ZOMBIE_WALK;
}

void CommonZombie::attack() {
    state = ATTACKING;
}

CommonZombie::CommonZombie(CommonZombie&& other)
    : Zombie(std::move(other)) {
}

CommonZombie::~CommonZombie() {}
