#include "player_state.h"
#include "./client/client_protocol.h"
#include "physics_manager.h"
#include <list>
#include <utility>
#include <arpa/inet.h>

PlayerState::PlayerState(
    const std::string &name,
    int id,
    int weapon_code,
    int16_t max_x, 
    int16_t max_y
) : 
GameEntity(name, max_x, max_y, CollisionLayer::Friendly) {
    this->id = id;
    this->hit_points = STARTING_HIT_POINTS;
    this->arma = new Arma1();
    this->rect_width = PLAYER_RECT_WIDTH;
    this->rect_height = PLAYER_RECT_HEIGHT;
    this->respawn_time = -1;
    if (weapon_code == 1)
        this->arma = new Arma1();
    if (weapon_code == 2)
        this->arma = new Arma2();
    if (weapon_code == 3)
        this->arma = new Arma3();
}

PlayerState::PlayerState(PlayerState &&other) : GameEntity(std::move(other)) {
    if (this == &other)
        return;
    hit_points = other.hit_points;
    arma = other.arma;

    other.arma = nullptr;
}

void PlayerState::on_collission_detected(GameEntity *other) {
    other->attack(this);
}

std::string PlayerState::get_name() {
    return name;
}

void PlayerState::take_damage(uint8_t damage) {
    std::cout << "baambaam" << std::endl;
    if (damage > hit_points) {
        hit_points = 0;
        //respawn_time = RESPAWN_TIME;
    } else {
        hit_points -= damage;
    } 
}

PlayerStateReference PlayerState::make_ref() {
    PlayerStateReference ref;
    ref.id = id;
    ref.hit_points = hit_points;
    ref.name = name;
    ref.rounds = arma->get_rounds();
    ref.state = state;
    ref.x = (uint16_t) position.x;
    ref.y = (uint16_t) position.y;
    ref.direction = facing_direction;
    return ref;
}

void PlayerState::attack(GameEntity *other) {
    //if (this->arma->try_shoot()) {
        //this->state = ATTACKING;
    //}//ya no se usa porque no puedo devolver la bala o pasar el vec por parametro
    
    //else
        //this->state = IDLE;
}

void PlayerState::move() {
    //estas lineas son para que la animacion de reloading se mantenga hasta que se termine de recargar
    //lo mismo para la de disparar

    int8_t delay = this->arma->get_delay();

    bool moved = GameEntity::move();
    if (moved) {
        if (this->state == IDLE) {
            this->state = MOVING;
        } else if ((this->state == RELOADING) && (delay == 0)) {
            this->state = MOVING;//termino la recarga
        } else if ((this->state == ATTACKING) && (delay == 0)) {
            this->state = MOVING;//termino el disparo
        } 
    } else {
        if (this->state == MOVING) {
            this->state = IDLE;
        } else if ((this->state == RELOADING) && (delay == 0)) {
            this->state = IDLE;//termino la recarga
        } else if ((this->state == ATTACKING) && (delay == 0)) {
            this->state = IDLE;//termino el disparo
        }
    }
}
/*
void PlayerState::shoot(int flag) {
    if (flag && this->rounds > 0) {
        this->rounds--;
        if (this->state == MOVING) {
            this->state = ATTACKING_AND_MOVING;
        } else if (this->state == IDLE) {
            this->state = ATTACKING;
        }
    } else {
        if (this->state == ATTACKING) {
            this->state = IDLE;
        } else if (this->state == ATTACKING_AND_MOVING) {
            this->state = MOVING;
        }
    }
}*/

void PlayerState::next_state(uint8_t cmd, std::list<Bullet>& vec, uint16_t& bullets) {
    if (this->hit_points == 0) {
        this->state = DEAD;
        this->arma->advance_time();
        return;
    }

    if (cmd == MOVE_DOWN) {
        direction.y = 1;
    } else if (cmd == MOVE_UP) {
        direction.y = -1;
    } else if (cmd == MOVE_RIGHT) {
        direction.x = 1;
        facing_direction = RIGHT;
    } else if (cmd == MOVE_LEFT) {
        direction.x = -1;
        facing_direction = LEFT;
    } else if (cmd == STOP_MOVING_DOWN) {
        direction.y = 0;
    } else if (cmd == STOP_MOVING_UP) {
        direction.y = 0;
    } else if (cmd == STOP_MOVING_RIGHT) {
        direction.x = 0;
    } else if (cmd == STOP_MOVING_LEFT) {
        direction.x = 0;
    } else if (cmd == RELOAD) {
        if (this->arma->try_reload())
            this->state = RELOADING;
        //else
            //this->state = IDLE;
    } else if (cmd == SHOOT) {
        if (this->arma->try_shoot()) {
            this->state = ATTACKING;
            this->arma->create_bullet(position,facing_direction,vec);
            bullets++;
            //el -64 es para que salga la bala del medio del modelo
        }
    } else if (cmd == STOP_SHOOTING) {
        //this->state = IDLE;
    } else if (cmd = THROW_GRENADE) {
        if (this->arma->try_grenade()) {
            this->arma->create_grenade(position,NULL);
            this->state = IDLE;
        }
    } else if (cmd = PREPARE_GRENADE) {
        int u = this->arma->charge_grenade();
        if (u==1)
            this->state = THROWING_GRENADE;
        if (u==2) {
            this->state = IDLE;
            take_damage(50);
            this->arma->create_grenade(position,NULL);
        }
    }

    this->move();
}

bool PlayerState::is_dead() {
    return hit_points == 0;
}

void PlayerState::pass_time() {
    this->arma->advance_time();
    //-1 seria que esta vivo
    //cuando muere, se pone un timer
    //cuando el timer baja a 0, el jugador revive
    if (this->hit_points == 0 && this->respawn_time > 0) {
        respawn_time--;
    } else if (this->hit_points == 0 && this->respawn_time == 0) {
        respawn_time = -1;
        this->hit_points = 100;
    } else if (this->hit_points == 0 && this->respawn_time == -1) {
        respawn_time = RESPAWN_TIME;
    }
}

// void PlayerState::as_response(protocol_response_t *response) {
//     response->state = this->state;
//     response->position[0] = htons(this->position[0]);
//     response->position[1] = htons(this->position[1]);
//     response->rounds = htons(this->rounds);
// }

PlayerState::~PlayerState() {
    if (arma) {
        delete arma;
        arma = nullptr;
    }
}
