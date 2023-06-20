#include "bullet.h"
#include "protocol_types.h"
#include "physics_manager.h"
#include "zombie.h"

Bullet::Bullet(uint8_t dmg, uint8_t bc, entity_direction direc, bool piercing, Vector2D position, bool falloff) :
    GameEntity(
        "Im a bullet", 
        position,
        DEFAULT_MAX_X,
        DEFAULT_MAX_Y,
        CollisionLayer::FriendlyProjectile
    ) {
    this->damage = dmg;
    this->piercing = piercing;
    this->facing_direction = direc;
    this->bullet_count = bc;
    this->state = 0;
    this->falloff = falloff;
}

void Bullet::move() {
    if (this->facing_direction == LEFT) {
        position.x-=95;
    } else {
        position.x+=95;
    }
    if (falloff)
        this->damage = damage * 4 / 3;

    //std::cout << "MOVED" << std::endl;
}

void Bullet::on_collission_detected(GameEntity *other) {
    if (this->bullet_count > 0 && this->damage > 0) {
        attack(other);
    }
}

void Bullet::attack(GameEntity *other) {
    Zombie *z = (Zombie*)other;
    if (piercing) {
        z->take_damage(damage);
        damage = damage / 2;
    } else {
        while ((bullet_count > 0) && (z->get_health() > 0)) {
            z->take_damage(damage);
            bullet_count--;
        }
    }
}

bool Bullet::is_off_scope() {
    return this->max_x < position.x;
}

PlayerStateReference Bullet::make_ref() {
    PlayerStateReference a;
    a.direction = facing_direction;
    a.x = position.x;
    a.y = position.y;
    a.name = "";
    a.hit_points = 0;
    a.state = state++;
    if (piercing)
        a.id = 156;//scout
    else if (falloff)
        a.id = 154;//idf
    else
        a.id = 155;//p90

    //importante, la bala solo hace sonido en el cliente cuando state = 0
    //osea, solo cuando se dispara por primera vez
    return a;
}

Vomit_Projectile::Vomit_Projectile(Vector2D position, entity_direction direc) :
    GameEntity(
        "Im vomit", 
        position,
        DEFAULT_MAX_X,
        DEFAULT_MAX_Y,
        CollisionLayer::HostileProjectile
    ) {
    this->facing_direction = direc;
    state = 0;
    this->rect_width = VENOM_PROJECTILE_SIZE;
    this->rect_height = VENOM_PROJECTILE_SIZE;
    this->position = position;
    dead = false;
}

void Vomit_Projectile::on_collission_detected(GameEntity *other) {
    if (!dead)
        attack(other);
}

void Vomit_Projectile::attack(GameEntity *other) {
    PlayerState *z = (PlayerState*)other;
    z->take_damage(VENOM_PROJECTILE_DAMAGE);
    dead = true;
}

bool Vomit_Projectile::is_dead() {
    return dead;
}

void Vomit_Projectile::move() {
    if (this->facing_direction == LEFT) {
        position.x -= VENOM_PROJECTILE_SPEED;
    } else {
        position.x += VENOM_PROJECTILE_SPEED;
    }
}

PlayerStateReference Vomit_Projectile::make_ref() {
    PlayerStateReference a;
    a.direction = facing_direction;
    a.x = position.x;
    a.y = position.y;
    a.name = "";
    a.hit_points = 0;
    a.state = 0;
    a.id = 101; //definido venom projectile en asset manager
    return a;
}