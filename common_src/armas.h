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
    uint8_t balas; // NO CONSTANTE, BALAS QUE QUEDAN
    uint8_t delay_recarga;//CONSTANTE, TIEMPO QUE TARDA EN RECARGAR
    uint8_t delay_disparo;//CONSTANTE, TIEMPO ENTRE DISPAROS
    uint8_t delay;  // NO CONSTANTE, DELAY QUE QUEDA PARA DISPARAR
    uint16_t g_delay_cte; //CONSTANTE, TIEMPO ENTRE GRANADAS
    uint16_t g_delay; // NO CONSTANTE, DELAY QUE QUEDA PARA GRANADA
    uint8_t throwing_distance; //NO CTE, DISTANCIA QUE CUBRE

    public:
    //constructor
    Arma(uint8_t c, uint8_t dr, uint8_t dd, uint16_t gd);

    //decrementa en 1 el delay
    void advance_time();

    //intenta disparar, lo logra si el delay es 0 y hay balas
    bool try_shoot();

    //intenta recargar, lo logra si el delay es 0 y faltan balas
    bool try_reload();

    //eso, la intenta tirar
    bool try_grenade();

    //intenta cargar, devuelve 0 si no esta listo, 1 si salio todo bien, 2 si le explota en la mano
    int charge_grenade();

    //getters
    uint8_t get_rounds();
    uint8_t get_delay();

    //crea una bala y la pushea a la lista de balas del GameLoop
    virtual void create_bullet(Vector2D position, entity_direction direc, std::list<Bullet>& vec) = 0;

    //crea una granada en la lista de granadas???
    virtual void create_grenade(Vector2D position, std::list<int>* gren/*TODO*/) = 0;
};

class Arma1 : public Arma { //IDF, Granada Explosiva
    public:
    explicit Arma1();
    void create_bullet(Vector2D position, entity_direction direc, std::list<Bullet>& vec) override;
    void create_grenade(Vector2D position, std::list<int>* gren/*TODO*/) override;
};

class Arma2 : public Arma { //P90, Bombardeo
    public:
    explicit Arma2();
    void create_bullet(Vector2D position, entity_direction direc, std::list<Bullet>& vec) override;
    void create_grenade(Vector2D position, std::list<int>* gren/*TODO*/) override;
};

class Arma3 : public Arma { //Scout, Smoke
    public:
    explicit Arma3();
    void create_bullet(Vector2D position, entity_direction direc, std::list<Bullet>& vec) override;
    void create_grenade(Vector2D position, std::list<int>* gren/*TODO*/) override;
};

#endif
