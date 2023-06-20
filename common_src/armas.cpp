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
        if (balas == 0) {
            this->try_reload();
        } else {
            delay = delay_disparo;
        }
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
    if (this->g_delay > 0) {
        this->g_delay--;
        //std::cout << "nuevo delay:" << std::to_string(g_delay) << std::endl;
    }  
}

Arma::Arma(uint8_t c, uint8_t dr, uint8_t dd, uint16_t gd) {
    this->cargador = c;
    this->balas = c;
    this->delay_recarga = dr;
    this->delay_disparo = dd;
    this->delay = 0;
    this->g_delay_cte = gd;
    this->g_delay = 0;
}

void Arma1::create_bullet(Vector2D position, entity_direction direc, std::list<Bullet>& vec) {
    vec.push_back(Bullet(ARMA1_DAMAGE,ARMA1_BULLET_COUNT,direc,false,position,true));
}

Arma1::Arma1() : Arma(ARMA1_MAGAZINE,ARMA1_RELOAD_DELAY,ARMA1_SHOOT_DELAY,ARMA1_GRENADE_DELAY) {}

void Arma2::create_bullet(Vector2D position, entity_direction direc, std::list<Bullet>& vec) {
    vec.push_back(Bullet(ARMA2_DAMAGE,ARMA2_BULLET_COUNT,direc,false,position));
}

Arma2::Arma2() : Arma(ARMA2_MAGAZINE,ARMA2_RELOAD_DELAY,ARMA2_SHOOT_DELAY,ARMA2_GRENADE_DELAY) {}

void Arma3::create_bullet(Vector2D position, entity_direction direc, std::list<Bullet>& vec) {
    vec.push_back(Bullet(ARMA3_DAMAGE,ARMA3_BULLET_COUNT,direc,true,position));
}

Arma3::Arma3() : Arma(ARMA3_MAGAZINE,ARMA3_RELOAD_DELAY,ARMA3_SHOOT_DELAY,ARMA3_GRENADE_DELAY) {}


bool Arma::try_grenade() {
    if (this->g_delay == 0) {
        this->g_delay = this->g_delay_cte;
        return true;
    }
    return false;
}

int Arma::charge_grenade() {
    if (this->g_delay == 0 && this->throwing_distance < 20) {
        this->throwing_distance++;
        return 1;
    } else if (this->g_delay == 0 && this->throwing_distance >= 20) {
        return 2;
    }
    if (this->g_delay > 0)
        return 0;
}

void Arma1::create_grenade(Vector2D position, std::list<int>* gren/*TODO*/) {
    //sumarla a la lista
    this->throwing_distance = 0;
}

void Arma2::create_grenade(Vector2D position, std::list<int>* gren/*TODO*/) {
    //sumarla a la lista
    this->throwing_distance = 0;
}

void Arma3::create_grenade(Vector2D position, std::list<int>* gren/*TODO*/) {
    //sumarla a la lista
    this->throwing_distance = 0;
}
