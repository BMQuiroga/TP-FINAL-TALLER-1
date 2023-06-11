#include "game_entity.h"
#include "./client/client_protocol.h"
#include <arpa/inet.h>

int GameEntity::next_id = 1;

GameEntity::GameEntity(const std::string &name, int16_t max_x, int16_t max_y) : 
name(name), direction(2, 0), x(2), y(0), facing_direction(RIGHT), speed(10) {
    this->id = next_id++;
    this->state = IDLE;
    this->max_x = max_x;
    this->max_y = max_y;
}

GameEntity::GameEntity(GameEntity &&other) {
    if (this == &other)
        return;

    name = std::move(other.name);
    direction = std::move(other.direction);
    x = other.x;
    y = other.y;
    facing_direction = other.facing_direction;
    speed = other.speed;
    id = other.id;
    state = other.state;
    max_x = other.max_x;
    max_y = other.max_y;
}

GameEntity::~GameEntity() {}

std::string GameEntity::get_name() {
    return name;
}

int8_t GameEntity::get_state() {
    return state;
}

bool GameEntity::move() {
    std::vector<uint16_t> prev_position({x, y});
    int16_t new_x = (int16_t)x + (speed * this->direction[0]);
    if (new_x < 0) {
        new_x = 0;
    } else if (new_x >= this->max_x) {
        new_x = this->max_x - 1;
    }
    int16_t new_y = (int16_t)y + (speed * this->direction[1]);
    if (new_y < 0) {
        new_y = 0;
    } else if (new_y >= this->max_y) {
        new_y = this->max_y - 1;
    }
    x = new_x;
    y = new_y;

    bool moved = false;
    if (this->direction[0] != 0) {
        if (prev_position[0] != x) {
            moved = true;
        }
    } else if (this->direction[1] != 0) {
        if (prev_position[1] != y) {
            moved = true;
        }
    }
    return moved;
}
