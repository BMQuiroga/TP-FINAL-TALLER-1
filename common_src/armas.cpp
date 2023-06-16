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

Arma::Arma(uint8_t c, uint8_t dr, uint8_t dd) {
    this->cargador = c;
    this->balas = c;
    this->delay_recarga = dr;
    this->delay_disparo = dd;
    this->delay = 0;
}

void Arma1::create_bullet(Vector2D position, entity_direction direc, std::list<Bullet>& vec) {
    vec.push_back(Bullet(5,20,direc,false,position));
}

Arma1::Arma1() : Arma(50,40,20) {}

void Arma2::create_bullet(Vector2D position, entity_direction direc, std::list<Bullet>& vec) {
    vec.push_back(Bullet(3,10,direc,false,position));
}

Arma2::Arma2() : Arma(30,10,10) {}

void Arma3::create_bullet(Vector2D position, entity_direction direc, std::list<Bullet>& vec) {
    vec.push_back(Bullet(150,1,direc,true,position));
}

Arma3::Arma3() : Arma(20,20,10) {}


