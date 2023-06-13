#include "zombie.h"
#include "physics_manager.h"
#include "player_state.h"
#include <string>
#include <utility>

Zombie::Zombie(
    const std::string &name,
    Vector2D position,
    GameEntity *target,
    int16_t max_x, 
    int16_t max_y
) : GameEntity(
    name,
    position,
    max_x, max_y,
    CollisionLayer::Hostile), target(target) {
    rect_width = ZOMBIE_RECT_HEIGHT;
    rect_height = ZOMBIE_RECT_HEIGHT;
    speed = 4;
}

Zombie::~Zombie() {}

void Zombie::move() {
    bool moved = GameEntity::move();
    if (moved) {
        state = MOVING;
    } else {
        state = IDLE;
    }
    std::cout << "my state is " << std::to_string(state) << std::endl;
}

void Zombie::on_collission_detected(GameEntity *other) {
    /*if (state != ATTACKING) {
        state = ATTACKING;
        // move towards the player and bite him
    } else {
        // collision
        PlayerState *player = (PlayerState*)other;
        //std::cout << "boomboom" << std::endl;
        player->take_damage(damage);
    }*/
}

void Zombie::set_id(int new_id) {
    id = new_id;
}

void Zombie::take_damage(uint8_t damage) {
    if (damage > health)
        health = 0;
    else
        health -= damage;
}

void Zombie::set_target(GameEntity *target) {
    this->target = target;
    Vector2D target_position = Vector2D(100, 2);
    has_target_set = true;
}

bool Zombie::has_target() {
    return has_target_set;
}

void Zombie::set_direction(int x, int y) {
    direction.x = x;
    //if (x == 0) {
        direction.y = y;
    //}
}

void Zombie::next_state() {
    Vector2D target_position = target->get_location();
    // Calculate the direction vector from the zombie to the player
    Vector2D target_direction = target_position - position;
    // Normalize the target direction if it's not too close to zero
    float threshold = 0.001f;
    float magnitude = target_direction.magnitude();
    if (magnitude > threshold) {
        target_direction.x /= magnitude;
        target_direction.y /= magnitude;
    } else {
        target_direction = Vector2D(0.0f, 0.0f);  // Set target direction to zero if too close to zero
    }
    // Adjust the current direction towards the target direction
    float dampingFactor = 0.1f;  // Adjust this value to control the damping effect
    direction += dampingFactor * (target_direction - direction);
    // Normalize the direction
    magnitude = direction.magnitude();
    if (magnitude > threshold) {
        direction.x /= magnitude;
        direction.y /= magnitude;
    } else {
        direction = Vector2D(0.0f, 0.0f);  // Set direction to zero if too close to zero
    }

    // Normalize the direction vector
    // direction = new_direction.normalized();
    // std::cout << "My new direction is " << direction.x << "and y " << direction.y << std::endl;
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
    : GameEntity(std::move(other)), target(other.target) {
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
    GameEntity *target,
    int16_t max_x, 
    int16_t max_y
) : Zombie(name, position, target, max_x, max_y) {
    id = 51;
    damage = 5;
    zombie_type = ZOMBIE;
    attack_type = ZOMBIE_BITE;
    movement_type = ZOMBIE_WALK;
}

void CommonZombie::attack(GameEntity *other) {
    //esto es un poco turbio pero funciona asi:
    //como siempre el zombie esta moving, siempre entraria al if y nunca al else
    //asi que las detecciones de colisiones del zombie y del player ambas hacen lo mismo,
    //una setea el flag y la otra como ya esta attacking, hace el daño
    /*if (state != ATTACKING) {
        state = ATTACKING;
        // move towards the player and bite him
    } else {*/
        // collision
        PlayerState *player = (PlayerState*)other;
        //std::cout << "boomboom22222" << std::endl;
        player->take_damage(damage);
    //}
}

CommonZombie::CommonZombie(CommonZombie&& other)
    : Zombie(std::move(other)) {
}

CommonZombie::~CommonZombie() {}
