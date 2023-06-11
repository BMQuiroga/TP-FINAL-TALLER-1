#ifndef __ARMA_GENERICA_H__
#define __ARMA_GENERICA_H__

#include <iostream>
#include "player_state.h"

class PlayerState;

class Arma {
    protected:
    //public:
    uint8_t cargador;//CONSTANTE, BALAS QUE TIENE 1 CARGADOR
    uint8_t balas;
    uint8_t delay_recarga;//CONSTANTE, TIEMPO QUE TARDA EN RECARGAR
    uint8_t delay_disparo;//CONSTANTE, TIEMPO ENTRE DISPAROS
    uint8_t delay;
    PlayerState * owner;

    public:
    Arma(uint8_t c, uint8_t dr, uint8_t dd, PlayerState * owner);
    void advance_time();
    bool try_shoot();
    bool try_reload();
    uint8_t get_rounds();
    uint8_t get_delay();
    virtual void create_bullet() = 0;
};

class Arma1 : public Arma {
    public:
    Arma1(PlayerState * owner);
    void create_bullet() override;
};

#endif
