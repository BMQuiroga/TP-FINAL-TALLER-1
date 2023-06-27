#include "bullet.h"
#include "protocol_types.h"
#include "physics_manager.h"
#include "zombie.h"

Bullet::Bullet(
    uint8_t dmg,
    uint8_t bc,
    entity_direction direc,
    bool piercing,
    Vector2D position,
    bool falloff,
    PhysicsManager *physics
) :
    GameEntity(
        "Im a bullet", 
        position,
        GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_X"),
        GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_Y"),
        CollisionLayer::FriendlyProjectile,
        physics
    ) {
    this->damage = dmg;

    position.x += GameConfig::get_instance()->get_value<int>("BULLET_SHOT_OFFSET");//offset para que la bala se genere a la altura del arma
    this->position = position;

    this->piercing = piercing;
    this->facing_direction = direc;
    this->bullet_count = bc;
    this->state = 0;
    this->falloff = falloff;
    this->rect_height = GameConfig::get_instance()->get_value<int>("BULLET_HITBOX");
    this->rect_width = GameConfig::get_instance()->get_value<int>("BULLET_HITBOX");
    std::cout << "MY HITBOX IS: " << this->rect_height << " x " << this->rect_width << std::endl;
}

void Bullet::move() {
    //std::cout << "bullet previously at: " << position.x << ", " << position.y;
    GameConfig *config = GameConfig::get_instance();
    if (this->facing_direction == LEFT) {
        position.x -= config->get_value<int>("ZOMBIE_RECT_WIDTH");
    } else {
        position.x += config->get_value<int>("ZOMBIE_RECT_WIDTH");
    }
    if (falloff)
        this->damage = damage * 3 / 4;
    //std::cout << " moved at: " << position.x << ", " << position.y << std::endl;
    //std::cout << "MOVED" << std::endl;
}

bool Bullet::is_dead() {
    return this->damage == 0 || this->bullet_count == 0 || is_off_scope();
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
        //std::cout << "PIERCING: Dealt " << std::to_string(damage) << " damage" << std::endl;
        damage = damage / 2;
    } else {
        while ((bullet_count > 0) && (z->get_health() > 0)) {
            z->take_damage(damage);
            //std::cout << "Dealt " << std::to_string(damage) << " damage" << std::endl;
            bullet_count--;
        }

    }
    std::cout << "Bullets left: " << std::to_string(bullet_count) << std::endl;
}

bool Bullet::is_off_scope() {
    return this->max_x < position.x || position.x < 0;
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
    return a;
}

Vomit_Projectile::Vomit_Projectile(Vector2D position, entity_direction direc, PhysicsManager *physics) :
    GameEntity(
        "Im vomit", 
        position,
        GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_X"),
        GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_Y"),
        CollisionLayer::HostileProjectile,
        physics
    ) {
    GameConfig *config = GameConfig::get_instance();
    this->facing_direction = direc;
    state = 0;
    this->rect_width = config->get_value<uint16_t>("VENOM_PROJECTILE_SIZE");
    this->rect_height = config->get_value<uint16_t>("VENOM_PROJECTILE_SIZE");
    position.y += 30;//para que dispare desde el pecho
    this->position = position;
    dead = false;
}

void Vomit_Projectile::on_collission_detected(GameEntity *other) {
    if (!dead)
        attack(other);
}

void Vomit_Projectile::attack(GameEntity *other) {
    GameConfig *config = GameConfig::get_instance();
    PlayerState *z = (PlayerState*)other;
    z->take_damage(config->get_value<uint8_t>("VENOM_PROJECTILE_DAMAGE"));
    dead = true;
}

bool Vomit_Projectile::is_dead() {
    return dead;
}

void Vomit_Projectile::move() {
    GameConfig *config = GameConfig::get_instance();
    if (this->facing_direction == LEFT) {
        position.x -= config->get_value<uint8_t>("VENOM_PROJECTILE_SPEED");
    } else {
        position.x += config->get_value<uint8_t>("VENOM_PROJECTILE_SPEED");
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
    if (dead)
        a.id = 161; //ruido en impacto
    return a;
}

PlayerStateReference Vomit_Projectile::make_noise_ref() {
    PlayerStateReference a;
    a.direction = facing_direction;
    a.x = position.x;
    a.y = position.y;
    a.name = "";
    a.hit_points = 0;
    a.state = this->state++;
    a.id = 161; //definido ruido de acido en asset manager
    return a;
}