#include "game_entity.h"
#include "./client/client_protocol.h"
#include <utility>
#include <arpa/inet.h>

// uint32_t GameEntity::next_id = 1;

GameEntity::GameEntity(const std::string &name, int16_t max_x, int16_t max_y, CollisionLayer layer) : 
GameEntity(name, Vector2D(2,0), max_x, max_y, layer) {}

GameEntity::GameEntity(
    const std::string &name,
    Vector2D position,
    int16_t max_x, int16_t max_y,
    CollisionLayer layer
) : name(name), direction(2, 0), position(position),
    facing_direction(RIGHT), speed(10), collision_layer(layer) {
    this->id = 0;
    this->physics_id = PhysicsManager::get_instance()->register_entity(this, collision_layer);
    this->state = IDLE;
    this->max_x = max_x;
    this->max_y = max_y;
}

GameEntity::GameEntity(GameEntity &&other) {
    if (this == &other)
        return;

    name = std::move(other.name);
    direction = std::move(other.direction);
    position = std::move(other.position);
    collision_layer = other.collision_layer;
    facing_direction = other.facing_direction;
    speed = other.speed;
    id = other.id;
    physics_id = PhysicsManager::get_instance()->register_entity(this, collision_layer);
    state = other.state;
    max_x = other.max_x;
    max_y = other.max_y;
    rect_width = other.rect_width;
    rect_height = other.rect_height;

    other.~GameEntity();
}

GameEntity::~GameEntity() {
    if (physics_id != 0) {
        PhysicsManager::get_instance()->unregister_entity(physics_id);
        physics_id = 0;
    }
}

std::string GameEntity::get_name() {
    return name;
}

int8_t GameEntity::get_state() {
    return state;
}

uint32_t GameEntity::get_id() {
    return id;
}

uint32_t GameEntity::get_physics_id() {
    return physics_id;
}

Vector2D GameEntity::get_location() {
    return position;
}

Vector2D GameEntity::get_direction() {
    return direction;
}

bool GameEntity::check_collision(GameEntity *other) {
    SDL_Rect _this, _other;
    _this.x = position.x;
    _this.y = position.y;
    _this.w = (int)rect_width;
    _this.h = (int)rect_height;
    _other.x = other->position.x;
    _other.y = other->position.y;
    _other.w = (int)other->rect_width;
    _other.h = (int)other->rect_height;
    return SDL_HasIntersection(&_this, &_other);
}

bool GameEntity::move() {
    Vector2D& pos = position;
    Vector2D prev_position(pos);
    float new_x = pos.x + (speed * direction.x);
    if (new_x < 0) {
        new_x = 0;
    } else if (new_x >= this->max_x) {
        new_x = this->max_x - 1;
    }
    float new_y = pos.y + (speed * direction.y);
    if (new_y < 0) {
        new_y = 0;
    } else if (new_y >= this->max_y) {
        new_y = this->max_y - 1;
    }
    position = Vector2D(new_x, new_y);

    bool moved = false;
    if (direction.x != 0) {
        if (prev_position.x != pos.x) {
            moved = true;
        }
    } else if (direction.y != 0) {
        if (prev_position.y != pos.y) {
            moved = true;
        }
    }
    return moved;
}
