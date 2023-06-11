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
    this->state = IDLE;
    this->max_x = max_x;
    this->max_y = max_y;
    this->hit_points = STARTING_HIT_POINTS;
    this->arma = new Arma1(this);
}

PlayerState::PlayerState(PlayerState &&other) : GameEntity(std::move(other)) {
    if (this == &other)
        return;

    id = other.id;
    state = other.state;
    max_x = other.max_x;
    max_y = other.max_y;
    hit_points = other.hit_points;
    arma = other.arma;

    other.arma = nullptr;
}

std::string PlayerState::get_name() {
    return name;
}

void PlayerState::take_damage(uint8_t damage) {
    if (damage > hit_points)
        hit_points = 0;
    else
        hit_points -= damage;
}

PlayerStateReference PlayerState::make_ref() {
    PlayerStateReference ref;
    ref.id = id;
    ref.hit_points = hit_points;
    ref.name = name;
    ref.rounds = arma->get_rounds();
    ref.state = state;
    ref.x = x;
    ref.y = y;
    ref.direction = facing_direction;
    return ref;
}

void PlayerState::attack() {
    if(this->arma->try_shoot())
        this->state = ATTACKING;
}

void PlayerState::move() {
    bool moved = GameEntity::move();
    if (moved) {
        if (this->state == IDLE) {
            this->state = MOVING;
        }
    } else {
        if (this->state == MOVING) {
            this->state = IDLE;
        }
    }
}
/*
void PlayerState::shoot(int flag) {
    if (flag && this->rounds > 0) {
        this->rounds--;
        if (this->state == MOVING) {
            this->state = ATTACKING_AND_MOVING;
        } else if (this->state == IDLE) {
            this->state = ATTACKING;
        }
    } else {
        if (this->state == ATTACKING) {
            this->state = IDLE;
        } else if (this->state == ATTACKING_AND_MOVING) {
            this->state = MOVING;
        }
    }
}*/

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
    } else if (cmd == RELOAD) {
        if (this->arma->try_reload())
            this->state = RELOADING;
        else
            this->state = IDLE;
    } else if (cmd == SHOOT) {
        attack();
    }
    this->move();
}

void PlayerState::pass_time() {
    this->arma->advance_time();
}

// void PlayerState::as_response(protocol_response_t *response) {
//     response->state = this->state;
//     response->position[0] = htons(this->position[0]);
//     response->position[1] = htons(this->position[1]);
//     response->rounds = htons(this->rounds);
// }

PlayerState::~PlayerState() {
    if (arma) {
        delete arma;
    }
}
