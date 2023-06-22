#include "zombie.h"
#include "physics_manager.h"
#include "player_state.h"
#include "math_helper.h"
#include <string>
#include <utility>
#include <vector>

Zombie::~Zombie() {}

Zombie* Zombie::get_random_zombie(int secure) {
    int q;
    if (secure == -1) {
        q = getRandomNumber(0,4);
    } else {
        q = secure;
    }
    int x = getRandomNumber(0, 800);  // Random X position within game area
    int y = getRandomNumber(0, 95);  // Random Y position within game area
    Vector2D position(x, y);
    if (q == 0) {
        return new CommonZombie("Common",position);
    } else if (q == 1) {
        return new Jumper("Jumper",position);
    } else if (q == 2) {
        return new Spear("Spear",position);
    } else if (q == 3) {
        return new Venom("Venom",position);
    } else if (q == 4) {
        return new Witch("Witch",position);
    }
    std::cout << "ERROR: GETRANDOMZOMBIE RETURNS NULL" << std::endl;
    return nullptr;
}

void Zombie::move() {
    bool moved = GameEntity::move();
    if (moved) {
        state = MOVING;
    } else {
        state = IDLE;
    }
    //std::cout << "my state is " << std::to_string(state) << std::endl;
}

void Zombie::on_collission_detected(GameEntity *other) {
    /*if (state != ATTACKING) {
        state = ATTACKING;
        // move towards the player and bite him
    } else {
        // collision
        PlayerState *player = (PlayerState*)other;
        //std::cout << "boomboom" << std::endl;
        player->take_damage(damage);
    }*/
}

void Zombie::set_id(int new_id) {
    id = new_id;
}

void Zombie::take_damage(uint8_t damage) {
    if (damage > health) {
        health = 0;
        state = DEAD;
    } else
        health -= damage;
}


void Zombie::set_direction(int x, int y) {
    direction.x = x;
    //if (x == 0) {
        direction.y = y;
    //}
    if (x < 0) {
        this->facing_direction = LEFT;
    } else {
        this->facing_direction = RIGHT;
    }
}
/*
void Zombie::next_state() {
    Vector2D target_position = target->get_location();
    // Calculate the direction vector from the zombie to the player
    Vector2D target_direction = target_position - position;
    // Normalize the target direction if it's not too close to zero
    float threshold = 0.001f;
    float magnitude = target_direction.magnitude();
    if (magnitude > threshold) {
        target_direction.x /= magnitude;
        target_direction.y /= magnitude;
    } else {
        target_direction = Vector2D(0.0f, 0.0f);  // Set target direction to zero if too close to zero
    }
    // Adjust the current direction towards the target direction
    float dampingFactor = 0.1f;  // Adjust this value to control the damping effect
    direction += dampingFactor * (target_direction - direction);
    // Normalize the direction
    magnitude = direction.magnitude();
    if (magnitude > threshold) {
        direction.x /= magnitude;
        direction.y /= magnitude;
    } else {
        direction = Vector2D(0.0f, 0.0f);  // Set direction to zero if too close to zero
    }

    // Normalize the direction vector
    // direction = new_direction.normalized();
    // std::cout << "My new direction is " << direction.x << "and y " << direction.y << std::endl;
}*/

uint8_t Zombie::get_damage()
{
    return damage;
}

uint8_t Zombie::get_health()
{
    return health;
}

Zombie::Zombie(Zombie&& other)
    : GameEntity(std::move(other)) {
    if (this == &other)
        return;
    damage = other.damage;
    zombie_type = other.zombie_type;
    attack_type = other.attack_type;
    movement_type = other.movement_type;
}

ZombieStateReference Zombie::make_ref()
{
    ZombieStateReference ref;
    ref.id = get_id();
    ref.damage = damage;
    ref.health = health;
    ref.zombie_type = zombie_type;
    ref.state = state;
    ref.attack_type = attack_type;
    ref.movement_type = movement_type;
    ref.x = (uint16_t)position.x;
    ref.y = (uint16_t)position.y;
    ref.direction = facing_direction;
    return ref;
}

CommonZombie::CommonZombie(
    const std::string &name,
    Vector2D position,
    int16_t max_x, 
    int16_t max_y
) : Zombie(name, position, max_x, max_y) {
    id = 51;
    damage = ZOMBIE_DAMAGE;
    zombie_type = ZOMBIE;
    attack_type = ZOMBIE_BITE;
    movement_type = ZOMBIE_WALK;
}

void Zombie::attack(GameEntity *other) {
    state = ATTACKING;
    PlayerState *player = (PlayerState*)other;
    player->take_damage(damage);
}

bool Zombie::try_dissapear() {
    if (health == 0)
        this->show_death_timer--;
    if (show_death_timer == 0)
        return true;
    return false;
}

void Zombie::process_smoke() {
    
}
       

CommonZombie::CommonZombie(CommonZombie&& other)
    : Zombie(std::move(other)) {
}

CommonZombie::~CommonZombie() {}

Zombie::Zombie(
    const std::string &name,
    Vector2D position,
    int16_t max_x, 
    int16_t max_y
) : GameEntity(
    name,
    position,
    max_x, max_y,
    CollisionLayer::Hostile) {
    rect_width = ZOMBIE_RECT_HEIGHT;
    rect_height = ZOMBIE_RECT_HEIGHT;
    speed = ZOMBIE_SPEED;
    seeking_distance = ZOMBIE_SEEKING_DISTANCE;
    show_death_timer = ZOMBIE_TIMER;
}

Spear::Spear(
    const std::string &name,
    Vector2D position,
    int16_t max_x, 
    int16_t max_y
) : Zombie(name, position, max_x, max_y) {
    id = 53;
    damage = SPEAR_DAMAGE;
    zombie_type = SPEAR;
    attack_type = ZOMBIE_ATTACK1;
    movement_type = ZOMBIE_WALK;
    rect_width = SPEAR_RECT_WIDTH;
    health = SPEAR_HP;
    speed = SPEAR_SPEED;
}

Jumper::Jumper(
    const std::string &name,
    Vector2D position,
    int16_t max_x, 
    int16_t max_y
) : Zombie(name, position, max_x, max_y), objetive(-1,-1) {
    id = 52;
    damage = JUMPER_DAMAGE;
    zombie_type = JUMPER;
    attack_type = ZOMBIE_JUMP;
    movement_type = ZOMBIE_WALK;
    seeking_distance = JUMPER_SEEKING_DISTANCE;
    health = JUMPER_HP;
    speed = JUMPER_SPEED;
    cooldown = 0;
}

Venom::Venom(
    const std::string &name,
    Vector2D position,
    int16_t max_x, 
    int16_t max_y
) : Zombie(name, position, max_x, max_y) {
    id = 54;
    damage = VENOM_DAMAGE;//daño melee
    zombie_type = VENOM;
    attack_type = ZOMBIE_ATTACK2;
    movement_type = ZOMBIE_WALK;
    seeking_distance = VENOM_SEEKING_DISTANCE;
    rect_width = VENOM_RECT_WIDTH;
    health = VENOM_HP;
    speed = VENOM_SPEED;
    cooldown = 0;
}

Witch::Witch(
    const std::string &name,
    Vector2D position,
    int16_t max_x, 
    int16_t max_y
) : Zombie(name, position, max_x, max_y) {
    id = 55;
    damage = 0;
    zombie_type = WITCH;
    attack_type = ZOMBIE_SCREAM;
    movement_type = ZOMBIE_WALK;
    seeking_distance = WITCH_SEEKING_DISTANCE;
    health = WITCH_HP;
    speed = 0;
}

int Zombie::calculate_next_movement(std::vector<PlayerState>& players) {
    if (this->health == 0)
        return CODE_NULL;
    float closest_x = 0;
    float closest_y = 0;
    Vector2D this_pos = get_location();
    int distance = 99999;
    for (GameEntity &player : players) {
        Vector2D vector = player.get_location();
        int new_d = calculateDistance(this_pos,vector);
        if (new_d < distance) {
            distance = new_d;
            closest_x = vector.x;
            closest_y = vector.y;
        }
    }
    if (distance < seeking_distance || this->health < zom_HP) {//en el caso de zomb
        float next_pos_x = this_pos.x - closest_x;
        float next_pos_y = this_pos.y - closest_y;
        int direction_x = next_pos_x > 0 ? -1 : next_pos_x < 0 ? 1 : 0;
        int direction_y = next_pos_y > 0 ? -1 : next_pos_y < 0 ? 1 : 0;
        set_direction(direction_x,direction_y);
    } else {
        set_direction(0,0);
    }   
    move();
    return CODE_NULL;
}

int Witch::calculate_next_movement(std::vector<PlayerState>& players) {
    if (this->health == 0)
        return CODE_NULL;
    if (state == IDLE) {
        int x = getRandomNumber(0,30);
        if (x == 2) {
            state = SCREAMING;
        }
    } else if (state == SCREAMING) {
        int x = getRandomNumber(0,10);
        if (x == 2) {
            return CODE_WITCH_SPAWN;
        }
    }
    return CODE_NULL;
}

int Jumper::calculate_next_movement(std::vector<PlayerState>& players) {
    if (this->health == 0)
        return CODE_NULL;
    if (this->state == HURT && cooldown > 0) {
        cooldown--;
    } else if (this->state == HURT && cooldown == 0) {
        this->state = IDLE;
    } else if (this->state == IDLE) {
        set_objetive(players);
    } else if (this->state == JUMPING) {
        if (jump()) { //termina el salto, entra en cooldown
            this->state = HURT;
            this->cooldown = 20;
            this->objetive.x = -1;
            this->objetive.y = -1;
        }
    }
    return CODE_NULL;
}

void Jumper::set_objetive(std::vector<PlayerState>& players) {
    float closest_x = 0;
    float closest_y = 0;
    Vector2D this_pos = get_location();
    int distance = 99999;
    for (GameEntity &player : players) {
        Vector2D vector = player.get_location();
        int new_d = calculateDistance(this_pos,vector);
        if (new_d < distance) {
            distance = new_d;
            closest_x = vector.x;
            closest_y = vector.y;
        }
    }
    if (distance < seeking_distance) {
        objetive.x = closest_x;
        objetive.y = closest_y;
        state = JUMPING;
    }
}

bool Jumper::jump() {
    //std::cout << "DIRECTION SET AT: (" << objetive.x << ", " << objetive.y << " )" << std::endl;
    //std::cout << "ACTUALLY AT: (" << position.x << ", " << position.y << " )" << std::endl;
    //std::cout << "0000000000000000000000000000000000000000000000000000" << std::endl;
    Vector2D this_pos = position;
    if (calculateDistance(this_pos,objetive) < 19) {
        return true;//termina el salto
    }
    float next_pos_x = this_pos.x - objetive.x;
    float next_pos_y = this_pos.y - objetive.y;
    int direction_x = next_pos_x > 0 ? -1 : next_pos_x < 0 ? 1 : 0;
    int direction_y = next_pos_y > 0 ? -1 : next_pos_y < 0 ? 1 : 0;
    set_direction(direction_x,direction_y);
    move();
    if (state == MOVING)
        state = JUMPING;
    return false;
}

int Venom::calculate_next_movement(std::vector<PlayerState>& players) {
    if (this->health == 0)
        return CODE_NULL;
    if (cooldown > 0)
        cooldown--;
    float closest_x = 0;
    float closest_y = 0;
    Vector2D this_pos = get_location();
    int distance = 99999;
    for (GameEntity &player : players) {
        Vector2D vector = player.get_location();
        int new_d = calculateDistance(this_pos,vector);
        if (new_d < distance) {
            distance = new_d;
            closest_x = vector.x;
            closest_y = vector.y;
        }
    }
    if (distance < seeking_distance) {
        float next_pos_x = this_pos.x - closest_x;
        float next_pos_y = this_pos.y - closest_y;
        int direction_y = next_pos_y > 0 ? -1 : next_pos_y < 0 ? 1 : 0;
        set_direction(0,direction_y);
        if (next_pos_x > 0)
            facing_direction = LEFT;//como no se mueve en el eje y, siempre estaria mirando hacia adelante, esto es un fix
        if (abs(next_pos_y) < 20) {
            if (cooldown == 0) {
                state = THROWING_GRENADE;
                cooldown = VENOM_PROJECTILE_COOLDOWN;
                return CODE_VENOM_PROJECTILE;
            } else {
                return CODE_NULL;
            }
        set_direction(0,direction_y);
        }
    } else {
        set_direction(0,0);
    }   
    move();
    return CODE_NULL;
}

void Jumper::attack(GameEntity * other) {
    if (state == JUMPING) {
        PlayerState *player = (PlayerState*)other;
        player->take_damage(damage);
    }
}

void Witch::attack(GameEntity * other) {
    //nothing
}
