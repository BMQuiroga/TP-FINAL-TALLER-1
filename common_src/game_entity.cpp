#include "game_entity.h"
#include "./client/client_protocol.h"
#include <arpa/inet.h>

int GameEntity::next_id = 1;

GameEntity::GameEntity(const std::string &name, int16_t max_x, int16_t max_y) : 
name(name), direction(2, 0), x(2), y(0), facing_direction(RIGHT), speed(5) {
    this->id = next_id++;
    this->state = IDLE;
    this->max_x = max_x;
    this->max_y = max_y;
}

GameEntity::~GameEntity() {}

std::string GameEntity::get_name() {
    return name;
}
void GameEntity::move() {
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
}
