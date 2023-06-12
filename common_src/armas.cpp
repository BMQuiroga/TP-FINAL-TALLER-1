#include "armas.h"
#include "player_state.h"
#include <list>

bool Arma::try_reload() {
    //std::cout << "try reload" << std::endl;
    //std::cout << "dr: " << std::to_string(this->delay_recarga)<< std::endl;
    //std::cout << "dd: " << std::to_string(this->delay_disparo)<< std::endl;
    if (delay == 0 && balas != cargador) {
        //std::cout << "reload" << std::endl;
        balas = cargador;
        delay = delay_recarga;
        return true;
    }
    return false;
}

bool Arma::try_shoot() {
    //std::cout << "try shoot" << std::endl;
    if (delay == 0 && balas > 0) {
        //std::cout << "shoot" << std::endl;
        balas--;
        delay = delay_disparo;
        return true;
    }
    return false;
}


uint8_t Arma::get_rounds() {
    return this->balas;
}

uint8_t Arma::get_delay() {
    return this->delay;
}

void Arma::advance_time() {
    //std::cout << "delay era:" << std::to_string(delay) << std::endl;
    if (this->delay > 0) {
        this->delay--;
        //std::cout << "nuevo delay:" << std::to_string(delay) << std::endl;
    }   
}

void Arma1::create_bullet(Vector2D position, entity_direction direc, std::list<Bullet>& vec) {
    //uint16_t x = this->owner->position.at(0);
    //uint16_t y = this->owner->position.at(1);
    //uint8_t direction = this->owner->facing_direction;
    vec.push_back(Bullet(25,1,direc,false,position));
}

Arma::Arma(uint8_t c, uint8_t dr, uint8_t dd, PlayerState * owner) {
    this->cargador = c;
    this->balas = c;
    this->delay_recarga = dr;
    this->delay_disparo = dd;
    this->delay = 0;
    this->owner = owner;
}

// Arma::Arma(Arma &&other) {
//     if (this == &other) {
//         return;
//     }
//     cargador = other.cargador;
//     balas = other.balas;
//     delay_recarga = other.delay_recarga;
//     delay_disparo = other.delay_disparo;
//     delay = other.delay;
//     owner = other.owner;
// }

Arma1::Arma1(PlayerState * owner) : Arma(10,20,2,owner) {}


