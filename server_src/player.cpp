#include "player.h"

#include <stdint.h>
#include <iostream>
#include <string>
#include <sstream>


Player::Player() {
}

PlayerState Player::get_current_state() {
    return this->currentState;
}

std::tuple<int, int> Player::get_location() {
    return std::make_tuple(this->coordenates_x, this->coordenates_y);
}

std::tuple<int, int> Player::get_direction() {
    return std::make_tuple(this->moving_direction_x, this->moving_direction_y);
}

bool Player::is_moving() {
    return this->currentState == MOVING 
        || this->currentState == SHOOTINGANDMOVING;
}

bool Player::is_shooting() {
    return this->currentState == SHOOTING 
        || this->currentState == SHOOTINGANDMOVING;
}

void Player::change_direction(int x, int y) {
    this->moving_direction_x =  x;
    this->moving_direction_y =  y; 
    if (x == 0 && y == 0) {
        this->currentState = IDLE;
    } else if (this->currentState == PlayerState::SHOOTING) {
        this->currentState = SHOOTINGANDMOVING;  
    } else {
        this->currentState = MOVING;
    }
}

void Player::move() {
    this->coordenates_x = this->coordenates_x + this->moving_direction_x;
    this->coordenates_y = this->coordenates_y + this->moving_direction_y;
}

void Player::start_shooting() {
    if (this->currentState == MOVING) {
        this->currentState = SHOOTINGANDMOVING;
        return;  
    }
    this->currentState = SHOOTING;
}

void Player::stop_shooting() {
    if (this->currentState == SHOOTINGANDMOVING) {
        this->currentState = MOVING;
        return;  
    }
    this->currentState = IDLE;
}

void Player::stop_moving() {
    this->currentState = IDLE;
}

void Player::reload() {
    this->currentState = IDLE;
}

