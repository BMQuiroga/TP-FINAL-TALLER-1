#include "armas.h"

bool Arma::try_reload() {
    if (delay == 0) {
        balas = cargador;
        delay = delay_recarga;
        return true;
    }
    return false;
}

bool Arma::try_shoot() {
    if (delay == 0) {
        balas--;
        delay = delay_disparo;
        this->create_bullet();
        return true;
    }
    return false;
}


uint8_t Arma::get_rounds() {
    return this->balas;
}

void Arma::advance_time() {
    if (delay > 0)
        delay--;
}

void Arma1::create_bullet() {
    //uint16_t x = this->owner->position.at(0);
    //uint16_t y = this->owner->position.at(1);
    //uint8_t direction = this->owner->facing_direction;

}

Arma1::Arma1(PlayerState * owner) {
    this->cargador = 10;
    this->balas = 10;
    this->delay_recarga = 30;
    this->delay_disparo = 2;
    this->delay = 0;
    this->owner = owner;
}



