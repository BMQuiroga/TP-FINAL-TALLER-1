#ifndef __ARMA_GENERICA_H__
#define __ARMA_GENERICA_H__

#include <iostream>
#include <list>
#include "bullet.h"
#include "granadas.h"
#include "math_helper.h"

#define HE_GRENADE 1
#define AIR_STRIKE 2
#define SMOKE_GRENADE 3

class Grenade;

class Bullet;

class GrenadeHolder {
    public:
    virtual int charge(int type) = 0;
    virtual bool try_grenade(int type) = 0;
    virtual void create(int id, Vector2D position, entity_direction direc, std::list<Grenade>& gren, PhysicsManager *physics) = 0;
    virtual void advance_time() = 0;
    virtual int damage_on_explode_on_hand(int type) = 0;
    virtual void make_ref(uint8_t& t1, uint8_t& t2) = 0;
};

class Arma {
    protected:
    //public:
    uint8_t cargador;//CONSTANTE, BALAS QUE TIENE 1 CARGADOR
    uint8_t balas; // NO CONSTANTE, BALAS QUE QUEDAN
    uint8_t delay_recarga;//CONSTANTE, TIEMPO QUE TARDA EN RECARGAR
    uint8_t delay_disparo;//CONSTANTE, TIEMPO ENTRE DISPAROS
    uint8_t delay;  // NO CONSTANTE, DELAY QUE QUEDA PARA DISPARAR
    GrenadeHolder * grenades;

    public:
    //constructor
    Arma(uint8_t c, uint8_t dr, uint8_t dd);

    ~Arma();

    //manda un PSR de recarga para que el cliente haga el ruido
    static PlayerStateReference make_reload();

    void make_ref(uint8_t& t1, uint8_t& t2);

    //decrementa en 1 el delay
    void advance_time();

    //intenta disparar, lo logra si el delay es 0 y hay balas
    bool try_shoot();

    //intenta recargar, lo logra si el delay es 0 y faltan balas
    bool try_reload();

    //eso, la intenta tirar
    bool try_grenade(int id);

    //intenta cargar, devuelve 0 si no esta listo, 1 si salio todo bien, 2 si le explota en la mano
    int charge_grenade(int type);

    //getters
    uint8_t get_rounds();
    uint8_t get_delay();

    //crea una bala y la pushea a la lista de balas del GameLoop
    virtual void create_bullet(Vector2D position, entity_direction direc, std::list<Bullet>& vec, PhysicsManager *physics) = 0;

    //crea una granada en la lista de granadas???
    void create_grenade(int id, Vector2D position, entity_direction direc, std::list<Grenade>& gren, PhysicsManager *physics);

    int damage_on_explode_on_hand(int type);
};

class Arma1 : public Arma { //IDF
    private:
    public:
    explicit Arma1();
    void create_bullet(Vector2D position, entity_direction direc, std::list<Bullet>& vec, PhysicsManager *physics) override;
};

class Arma2 : public Arma { //P90, Bombardeo
    public:
    explicit Arma2();
    void create_bullet(Vector2D position, entity_direction direc, std::list<Bullet>& vec, PhysicsManager *physics) override;
};

class Arma3 : public Arma { //Scout
    public:
    explicit Arma3();
    void create_bullet(Vector2D position, entity_direction direc, std::list<Bullet>& vec, PhysicsManager *physics) override;
};

class DefaultGH : public GrenadeHolder {
    private:
    int e_delay;
    int e_delay_cte;
    int s_delay;
    int s_delay_cte;
    int e_distance;
    int s_distance;
    public:
    DefaultGH();
    int charge(int type);
    bool try_grenade(int type);
    void create(int id, Vector2D position, entity_direction direc, std::list<Grenade>& gren, PhysicsManager *physics);
    void advance_time();
    int damage_on_explode_on_hand(int type);
    void make_ref(uint8_t& t1, uint8_t& t2);
};

class Bombarder :  public GrenadeHolder {
    private:
    int delay;
    int delay_cte;
    public:
    Bombarder();
    int charge(int type);
    bool try_grenade(int type);
    void create(int id, Vector2D position, entity_direction direc, std::list<Grenade>& gren, PhysicsManager *physics);
    void advance_time();
    int damage_on_explode_on_hand(int type);
    void make_ref(uint8_t& t1, uint8_t& t2);
};


#endif
