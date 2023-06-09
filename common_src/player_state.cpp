#include "player_state.h"
#include "./client/client_protocol.h"
#include <arpa/inet.h>

PlayerState::PlayerState(
    const std::string &name, 
    int16_t max_x, 
    int16_t max_y
) : 
GameEntity(name, max_x, max_y) {
    this->id = 1;
    this->rounds = GUN_MAGAZINE_SIZE;
    this->state = IDLE;
    this->max_x = max_x;
    this->max_y = max_y;
    this->hit_points = STARTING_HIT_POINTS;
}

std::string PlayerState::get_name() {
    return name;
}

PlayerStateReference PlayerState::make_ref() {
    PlayerStateReference ref;
    ref.id = id;
    ref.hit_points = hit_points;
    ref.name = name;
    ref.rounds = rounds;
    ref.state = state;
    ref.x = x;
    ref.y = y;
    ref.direction = facing_direction;
    return ref;
}

void PlayerState::attack() {}

void PlayerState::move() {
    std::vector<uint16_t> prev_position({x, y});
    GameEntity::move();

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

    if (moved) {
        if (this->state == IDLE) {
            this->state = MOVING;
        } else if (this->state == ATTACKING) {
            this->state = ATTACKING_AND_MOVING;
        }
    } else {
        if (this->state == ATTACKING_AND_MOVING) {
            this->state = ATTACKING;
        } else if (this->state == MOVING) {
            this->state = IDLE;
        }
    }
}

// void PlayerState::shoot(int flag) {
//     if (flag && this->rounds > 0) {
//         this->rounds--;
//         if (this->state == MOVING) {
//             this->state = ATTACKING_AND_MOVING;
//         } else if (this->state == IDLE) {
//             this->state = ATTACKING;
//         }
//     } else {
//         if (this->state == ATTACKING) {
//             this->state = IDLE;
//         } else if (this->state == ATTACKING_AND_MOVING) {
//             this->state = MOVING;
//         }
//     }
// }

void PlayerState::next_state(uint8_t cmd) {
    if (cmd == MOVE_DOWN) {
        direction[1] = 1;
    } else if (cmd == MOVE_UP) {
        direction[1] = -1;
    } else if (cmd == MOVE_RIGHT) {
        direction[0] = 1;
        facing_direction = RIGHT;
    } else if (cmd == MOVE_LEFT) {
        direction[0] = -1;
        facing_direction = LEFT;
    } else if (cmd == STOP_MOVING_DOWN) {
        direction[1] = 0;
    } else if (cmd == STOP_MOVING_UP) {
        direction[1] = 0;
    } else if (cmd == STOP_MOVING_RIGHT) {
        direction[0] = 0;
    } else if (cmd == STOP_MOVING_LEFT) {
        direction[0] = 0;
    }
    this->move();
}

PlayerState::~PlayerState() {}
