#ifndef __ARMA_GENERICA_H__
#define __ARMA_GENERICA_H__

#include <iostream>
#include <list>
#include "bullet.h"
#include "math_helper.h"

class Arma {
    protected:
    //public:
    uint8_t cargador;//CONSTANTE, BALAS QUE TIENE 1 CARGADOR
    uint8_t balas;
    uint8_t delay_recarga;//CONSTANTE, TIEMPO QUE TARDA EN RECARGAR
    uint8_t delay_disparo;//CONSTANTE, TIEMPO ENTRE DISPAROS
    uint8_t delay;

    public:
    //constructor
    Arma(uint8_t c, uint8_t dr, uint8_t dd);

    //decrementa en 1 el delay
    // Arma (Arma&&);
    void advance_time();

    //intenta disparar, lo logra si el delay es 0 y hay balas
    bool try_shoot();

    //intenta recargar, lo logra si el delay es 0 y faltan balas
    bool try_reload();

    //getters
    uint8_t get_rounds();
    uint8_t get_delay();

    //crea una bala y la pushea a la lista de balas del GameLoop
    virtual void create_bullet(Vector2D position, entity_direction direc, std::list<Bullet>& vec) = 0;
};

class Arma1 : public Arma {
    public:
    explicit Arma1();
    void create_bullet(Vector2D position, entity_direction direc, std::list<Bullet>& vec) override;
};

class Arma2 : public Arma {
    public:
    explicit Arma2();
    void create_bullet(Vector2D position, entity_direction direc, std::list<Bullet>& vec) override;
};

class Arma3 : public Arma {
    public:
    explicit Arma3();
    void create_bullet(Vector2D position, entity_direction direc, std::list<Bullet>& vec) override;
};

#endif
