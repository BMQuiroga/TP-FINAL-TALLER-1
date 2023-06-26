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

PlayerStateReference Arma::make_reload() {
    PlayerStateReference a;
    a.id = 160;
    a.x = 0;
    a.y = 0;
    a.state = 0;
    a.hit_points = 0;
    return a;
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
    this->grenades->advance_time();
}


Arma::Arma(uint8_t c, uint8_t dr, uint8_t dd) {
    this->cargador = c;
    this->balas = c;
    this->delay_recarga = dr;
    this->delay_disparo = dd;
    this->delay = 0;
}

Arma::~Arma() {
    if (grenades) {
        delete grenades;
        grenades = nullptr;
    }
}

void Arma1::create_bullet(Vector2D position, entity_direction direc, std::list<Bullet>& vec, PhysicsManager *physics) {
    //position.y -= 80;//para que dispare desde el pecho
    vec.push_back(Bullet(ARMA1_DAMAGE,ARMA1_BULLET_COUNT,direc,false,position,true,physics));
}

Arma1::Arma1() : Arma(ARMA1_MAGAZINE,ARMA1_RELOAD_DELAY,ARMA1_SHOOT_DELAY) {
    this->grenades = new DefaultGH();
}

void Arma2::create_bullet(Vector2D position, entity_direction direc, std::list<Bullet>& vec, PhysicsManager *physics) {
    //position.y -= 80;//para que dispare desde el pecho
    vec.push_back(Bullet(ARMA2_DAMAGE,ARMA2_BULLET_COUNT,direc,false,position));
}

Arma2::Arma2() : Arma(ARMA2_MAGAZINE,ARMA2_RELOAD_DELAY,ARMA2_SHOOT_DELAY) {
    this->grenades = new Bombarder();
}

void Arma3::create_bullet(Vector2D position, entity_direction direc, std::list<Bullet>& vec, PhysicsManager *physics) {
    //position.y -= 80;//para que dispare desde el pecho
    vec.push_back(Bullet(ARMA3_DAMAGE,ARMA3_BULLET_COUNT,direc,true,position));
}

Arma3::Arma3() : Arma(ARMA3_MAGAZINE,ARMA3_RELOAD_DELAY,ARMA3_SHOOT_DELAY) {
    this->grenades = new DefaultGH();
}

bool Arma::try_grenade(int id) {
    return this->grenades->try_grenade(id);
}


int Arma::charge_grenade(int type) {
    return this->grenades->charge(type);
}

void Arma::create_grenade(int type, Vector2D position, entity_direction direc, std::list<Grenade>& gren, PhysicsManager *physics) {
    this->grenades->create(type, position, direc, gren, physics);
}


int Arma::damage_on_explode_on_hand(int type) {
    return this->grenades->damage_on_explode_on_hand(type);
}

void DefaultGH::advance_time() {
    if (this->e_delay > 0) {
        this->e_delay--;
        //std::cout << "nuevo delay:" << std::to_string(g_delay) << std::endl;
    } 
    if (this->s_delay > 0) {
        this->s_delay--;
        //std::cout << "nuevo delay:" << std::to_string(g_delay) << std::endl;
    } 
}

void Bombarder::advance_time() {
    if (this->delay > 0) {
        this->delay--;
        //std::cout << "nuevo delay:" << std::to_string(g_delay) << std::endl;
    } 

}

void DefaultGH::create(int type, Vector2D position, entity_direction direc, std::list<Grenade>& gren, PhysicsManager *physics) {
    if (direc == LEFT) {
        if (type == HE_GRENADE)
            gren.push_back(Grenade(HE_GRENADE, position.x - (GRANADA_FUERZA*e_distance), position.y, e_distance == 0, physics));
        if (type == SMOKE_GRENADE)
            gren.push_back(Grenade(SMOKE_GRENADE, position.x - (GRANADA_FUERZA*s_distance), position.y, s_distance == 0, physics));
    } else {
        if (type == HE_GRENADE)
            gren.push_back(Grenade(HE_GRENADE, position.x + (GRANADA_FUERZA*e_distance), position.y, e_distance == 0, physics));
        if (type == SMOKE_GRENADE)
            gren.push_back(Grenade(SMOKE_GRENADE, position.x + (GRANADA_FUERZA*s_distance), position.y, s_distance == 0, physics));
    }
    this->e_distance = 0;
    this->s_distance = 0;
}

void Bombarder::create(int id, Vector2D position, entity_direction direc, std::list<Grenade>& gren, PhysicsManager *physics) {
    int i = DEFAULT_MAX_X;
    int x = position.x;
    int safe_space = GRANADA_SIZE * 3;
    for (int j = 300; j < DEFAULT_MAX_X; j += BOMBARDER_SPACE_IN_BETWEEN) {
        if (abs(j - x) > safe_space)
            gren.push_back(Grenade(AIR_STRIKE, j, getRandomNumber(0,DEFAULT_MAX_Y), false, physics));
    }
    this->delay = delay_cte;
}

int DefaultGH::damage_on_explode_on_hand(int type) {
    return type == 1 ? GRANADA_DAMAGE : SMOKE_DAMAGE;
}

int Bombarder::damage_on_explode_on_hand(int type) {
    return 0;
}

bool DefaultGH::try_grenade(int id) {
    if (id == HE_GRENADE) {
        if (this->e_delay == 0) {
            this->e_delay = this->e_delay_cte;
            return true;
        }
        return false;
    } 
    if (id == SMOKE_GRENADE) {
        if (this->s_delay == 0) {
            this->s_delay = this->s_delay_cte;
            return true;
        }
        return false;
    }
    return false;
}

bool Bombarder::try_grenade(int id) {
    if (this->delay == 0) {
        this->delay = this->delay_cte;
        return true;
    }
    return false;
}

int DefaultGH::charge(int type) {
    if (type == HE_GRENADE) {
        if (this->e_delay == 0 && this->e_distance < 20) {
            this->e_distance++;
            return 1;
        } else if (this->e_delay == 0 && this->e_distance >= 20) {
            this->e_distance = 0;
            return 2;
        }
        return 0;
    }
    if (type == SMOKE_GRENADE) {
        if (this->s_delay == 0 && this->s_distance < 20) {
            this->s_distance++;
            return 1;
        } else if (this->s_delay == 0 && this->s_distance >= 20) {
            this->s_distance = 0;
            return 2;
        }
        return 0;
    }
    return 0;
}

int Bombarder::charge(int type) {
    return 0;
}

Bombarder::Bombarder() {
    this->delay = 0;
    this->delay_cte = ARMA2_GRENADE_DELAY;
}

DefaultGH::DefaultGH() {
    this->e_delay = 0;
    this->s_delay = 0;
    this->e_distance = 0;
    this->s_distance = 0;
    this->s_delay_cte = ARMA3_GRENADE_DELAY;
    this->e_delay_cte = ARMA1_GRENADE_DELAY;
}

void Arma::make_ref(uint8_t& t1, uint8_t& t2) {
    this->grenades->make_ref(t1,t2);
}

void DefaultGH::make_ref(uint8_t& t1, uint8_t& t2) {
    t1 = e_delay * 100 / ARMA1_GRENADE_DELAY;
    t2 = s_delay * 100 / ARMA3_GRENADE_DELAY;
}

void Bombarder::make_ref(uint8_t& t1, uint8_t& t2) {
    t1 = delay * 100 / ARMA2_GRENADE_DELAY;
    t2 = 255;//constante, cliente con esto entiende que es el bombarder
}