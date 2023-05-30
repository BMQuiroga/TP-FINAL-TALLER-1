#include "player_state.h"
#include "./client/client_protocol.h"
#include <arpa/inet.h>

PlayerState::PlayerState(const std::string &name, int16_t max_x, int16_t max_y) : 
name(name), direction(2, 0), position(2, 0) {
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
    ref.x = position[0];
    ref.y = position[1];
    ref.direction = direction[0] == 1 ? RIGHT : LEFT;
    return ref;
}

void PlayerState::move() {
    std::vector<uint16_t> prev_position(this->position);
    int16_t new_x = (int16_t)this->position[0] + this->direction[0];
    if (new_x < 0) {
        new_x = 0;
    } else if (new_x >= this->max_x) {
        new_x = this->max_x - 1;
    }
    int16_t new_y = (int16_t)this->position[1] + this->direction[1];
    if (new_y < 0) {
        new_y = 0;
    } else if (new_y >= this->max_y) {
        new_y = this->max_y - 1;
    }
    this->position[0] = new_x;
    this->position[1] = new_y;

    bool moved = false;
    if (this->direction[0] != 0) {
        if (prev_position[0] != this->position[0]) {
            moved = true;
        }
    } else if (this->direction[1] != 0) {
        if (prev_position[1] != this->position[1]) {
            moved = true;
        }
    }

    if (moved) {
        if (this->state == IDLE) {
            this->state = MOVING;
        } else if (this->state == SHOOTING) {
            this->state = SHOOTING_AND_MOVING;
        }
    } else {
        if (this->state == SHOOTING_AND_MOVING) {
            this->state = SHOOTING;
        } else if (this->state == MOVING) {
            this->state = IDLE;
        }
    }
}

void PlayerState::shoot(int flag) {
    if (flag && this->rounds > 0) {
        this->rounds--;
        if (this->state == MOVING) {
            this->state = SHOOTING_AND_MOVING;
        } else if (this->state == IDLE) {
            this->state = SHOOTING;
        }
    } else {
        if (this->state == SHOOTING) {
            this->state = IDLE;
        } else if (this->state == SHOOTING_AND_MOVING) {
            this->state = MOVING;
        }
    }
}

void PlayerState::next_state(int cmd) {
    if (cmd == MOVE_DOWN) {
        direction[1] = 1;
    } else if (cmd == MOVE_UP) {
        direction[1] = -1;
    } else if (cmd == MOVE_RIGHT) {
        direction[0] = 1;
    } else if (cmd == MOVE_LEFT) {
        direction[0] = -1;
    }
    this->move();
    // switch (req->cmd) {
    //     case NOP:
    //         if (this->state == SHOOTING) {
    //             this->shoot(1);
    //         } else if (this->state == MOVING) {
    //             this->move();
    //         } else if (this->state == SHOOTING_AND_MOVING) {
    //             this->move();
    //             this->shoot(1);
    //         }
    //         break;

    //     case SHOOT:
    //         this->shoot(req->args[0]);
    //         if (this->state == MOVING || this->state == SHOOTING_AND_MOVING) {
    //             this->move();
    //         }
    //         break;

    //     case MOVE:
    //         this->direction[0] = req->args[0];
    //         this->direction[1] = req->args[1];
    //         this->move();
    //         if (this->state == SHOOTING || this->state == SHOOTING_AND_MOVING) {
    //             this->shoot(1);
    //         }
    //         break;

    //     case RELOAD:
    //         this->state = IDLE;
    //         this->rounds = GUN_MAGAZINE_SIZE;
    //         break;
    //     }
}

// void PlayerState::as_response(protocol_response_t *response) {
//     response->state = this->state;
//     response->position[0] = htons(this->position[0]);
//     response->position[1] = htons(this->position[1]);
//     response->rounds = htons(this->rounds);
// }

PlayerState::~PlayerState() {}
