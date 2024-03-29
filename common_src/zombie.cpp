#include "zombie.h"
#include "physics_manager.h"
#include "player_state.h"
#include "math_helper.h"
#include <string>
#include <utility>
#include <vector>

Zombie::~Zombie() {}

Zombie* Zombie::get_random_zombie(int secure, PhysicsManager *physics, std::vector<PlayerState>& players) {
    int q;
    if (secure == -1) {
        q = getRandomNumber(0,4);
    } else if (secure < 5) {
        q = secure;
    } else if (secure == 5) {
        q = getRandomNumber(0,3);
    }
    int distance;
    int x,y;

    do {
        distance = 99999;
        x = getRandomNumber(0, GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_X"));  // Random X position within game area
        y = getRandomNumber(0, GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_Y"));  // Random Y position within game area
        Vector2D this_pos(x, y);
        for (GameEntity &player : players) {
            Vector2D vector = player.get_location();
            int new_d = calculateDistance(this_pos,vector);
            if (new_d < distance) {
                distance = new_d;
            }
        } 
    } while (distance < GameConfig::get_instance()->get_value<int>("SPAWNER_SAFE_AREA_X"));
    
    Vector2D this_pos(x, y);
    if (q == 0) {
        return new CommonZombie("Common", this_pos, physics);
    } else if (q == 1) {
        return new Jumper("Jumper", this_pos, physics);
    } else if (q == 2) {
        return new Spear("Spear",this_pos, physics);
    } else if (q == 3) {
        return new Venom("Venom", this_pos, physics);
    } else if (q == 4) {
        return new Witch("Witch", this_pos, physics);
    }
    std::cout << "ERROR: GETRANDOMZOMBIE RETURNS NULL" << std::endl;
    return nullptr;
}

void Zombie::generate_clear_the_area(int zombies, std::list<Zombie*>& list, PhysicsManager *physics, std::vector<PlayerState>& players) {
    for (int i = 0; i < (zombies/2); i++) {
        list.push_back(get_random_zombie(0, physics, players));
    }
    for (int i = 0; i < (zombies/2); i++) {
        list.push_back(get_random_zombie(-1, physics, players));
    }
    std::cout << "GENERATED CLEAR THE AREA WITH " << zombies << " zombies, " << (zombies/2) << " commond and " << (zombies/2)<< " random" << std::endl;
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
    //std::cout << "zombie ocd" << std::endl;
    attack(other);
}

void Zombie::set_id(int new_id) {
    id = new_id;
}

void Zombie::take_damage(uint8_t damage) {
    //std::cout << "ZOMBIE TD" << std::endl;
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
    int16_t max_y,
    PhysicsManager *physics
) : Zombie(name, position, max_x, max_y, physics) {
    GameConfig *config = GameConfig::get_instance();
    id = 51;
    damage = config->get_value<int>("ZOMBIE_DAMAGE");;
    zombie_type = ZOMBIE;
    attack_type = ZOMBIE_BITE;
    movement_type = ZOMBIE_WALK;
    health = GameConfig::get_instance()->get_value<int>("ZOMBIE_HP");
}

CommonZombie::CommonZombie(
    const std::string &name,
    Vector2D position,
    PhysicsManager *physics
) : CommonZombie(name, position, GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_X"), GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_Y"), physics) {}

CommonZombie::CommonZombie(CommonZombie&& other)
    : Zombie(std::move(other)) {
}

CommonZombie::~CommonZombie() {}

void Zombie::attack(GameEntity *other) {
    std::cout << "ZOMBIE ATTACK" << std::endl;
    if (this->health > 0) {
        state = ATTACKING;
        other->take_damage(damage);
    }
}

bool Zombie::try_dissapear() {
    if (health == 0)
        this->show_death_timer--;
    if (show_death_timer == 0)
        return true;
    return false;
}

void Zombie::process_smoke() {
    GameConfig *config = GameConfig::get_instance();
    this->smoked_time =  config->get_value<int>("ZOMBIE_IMPAIRED_TIME");
}

Zombie::Zombie(
    const std::string &name,
    Vector2D position,
    int16_t max_x, 
    int16_t max_y,
    PhysicsManager *physics
) : GameEntity(
    name,
    position,
    max_x, max_y,
    CollisionLayer::Hostile,
    physics
) {
    GameConfig *config = GameConfig::get_instance();
    rect_width = config->get_value<int>("ZOMBIE_RECT_WIDTH");
    rect_height = config->get_value<int>("ZOMBIE_RECT_HEIGHT");
    speed = config->get_value<int>("ZOMBIE_SPEED");
    seeking_distance = config->get_value<int>("ZOMBIE_SEEKING_DISTANCE");
    show_death_timer = GameConfig::get_instance()->get_value<int>("ZOMBIE_TIMER");
}

Zombie::Zombie(
    const std::string &name,
    Vector2D position,
    PhysicsManager *physics
) : Zombie(name, position, GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_X"), GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_Y"), physics) {}

Spear::Spear(
    const std::string &name,
    Vector2D position,
    int16_t max_x, 
    int16_t max_y,
    PhysicsManager *physics
) : Zombie(name, position, max_x, max_y, physics) {
    GameConfig *config = GameConfig::get_instance();
    id = 53;
    damage = config->get_value<int>("SPEAR_DAMAGE");
    zombie_type = SPEAR;
    attack_type = ZOMBIE_ATTACK1;
    movement_type = ZOMBIE_WALK;
    health = config->get_value<int>("SPEAR_HP");
    speed = config->get_value<int>("SPEAR_SPEED");
    seeking_distance = config->get_value<int>("SPEAR_SEEKING_DISTANCE");
    rect_width = config->get_value<int>("SPEAR_RECT_WIDTH");
    rect_height = config->get_value<int>("SPEAR_RECT_HEIGHT");
}

Spear::Spear(
    const std::string &name,
    Vector2D position,
    PhysicsManager *physics
) : Spear(name, position, GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_X"), GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_Y"), physics) {}

Spear::~Spear() {}

Jumper::Jumper(
    const std::string &name,
    Vector2D position,
    int16_t max_x, 
    int16_t max_y,
    PhysicsManager *physics
) : Zombie(name, position, max_x, max_y, physics), objetive(-1,-1) {
    GameConfig *config = GameConfig::get_instance();
    id = 52;
    damage = config->get_value<int>("JUMPER_DAMAGE");
    zombie_type = JUMPER;
    attack_type = ZOMBIE_JUMP;
    movement_type = ZOMBIE_WALK;
    seeking_distance = config->get_value<int>("JUMPER_SEEKING_DISTANCE");
    health = config->get_value<int>("JUMPER_HP");
    speed = config->get_value<int>("JUMPER_SPEED");
    cooldown = 0;
}

Jumper::Jumper(
    const std::string &name,
    Vector2D position,
    PhysicsManager *physics
) : Jumper(name, position, GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_X"), GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_Y"), physics) {}

Jumper::~Jumper() {}

Venom::Venom(
    const std::string &name,
    Vector2D position,
    int16_t max_x, 
    int16_t max_y,
    PhysicsManager *physics
) : Zombie(name, position, max_x, max_y, physics) {
    GameConfig *config = GameConfig::get_instance();
    id = 54;
    damage = config->get_value<uint8_t>("VENOM_DAMAGE");//daño melee
    zombie_type = VENOM;
    attack_type = ZOMBIE_ATTACK2;
    movement_type = ZOMBIE_WALK;
    seeking_distance = config->get_value<int>("VENOM_SEEKING_DISTANCE");
    health = config->get_value<uint8_t>("VENOM_HP");
    speed = config->get_value<uint16_t>("VENOM_SPEED");
    rect_width = config->get_value<int>("VENOM_RECT_WIDTH");
    rect_height = config->get_value<int>("VENOM_RECT_HEIGHT");
    cooldown = 0;
}

Venom::Venom(
    const std::string &name,
    Vector2D position,
    PhysicsManager *physics
) : Venom(name, position, GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_X"), GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_Y"), physics) {}

Venom::~Venom() {}

Witch::Witch(
    const std::string &name,
    Vector2D position,
    int16_t max_x, 
    int16_t max_y,
    PhysicsManager *physics
) : Zombie(name, position, max_x, max_y, physics) {
    GameConfig *config = GameConfig::get_instance();
    id = 55;
    damage = 0;
    zombie_type = WITCH;
    attack_type = ZOMBIE_SCREAM;
    movement_type = ZOMBIE_WALK;
    seeking_distance = config->get_value<int>("WITCH_SEEKING_DISTANCE");
    health = config->get_value<int>("WITCH_HP");
    speed = 0;
}

Witch::Witch(
    const std::string &name,
    Vector2D position,
    PhysicsManager *physics
) : Witch(name, position, GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_X"), GameConfig::get_instance()->get_value<int>("DEFAULT_MAX_Y"), physics) {}

Witch::~Witch() {}

int Zombie::calculate_next_movement(std::vector<PlayerState>& players) {
    if (this->id == 53 && this->health == 0) {//particlarmente, el spear no se muere
        state = DEAD;
        return GameConfig::get_instance()->get_value<int>("CODE_NULL");
    }
    bool impaired = false;
    if (this->health == 0)
        return GameConfig::get_instance()->get_value<int>("CODE_NULL");
    if (this->smoked_time > 0) {
        impaired = true;
        smoked_time--;
        speed = speed / 2;
    }
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
    if (distance < seeking_distance || this->health < GameConfig::get_instance()->get_value<int>("ZOMBIE_HP")) {
        float next_pos_x = this_pos.x - closest_x;
        float next_pos_y = this_pos.y - closest_y;
        int direction_x = next_pos_x > 0 ? -1 : next_pos_x < 0 ? 1 : 0;
        int direction_y = next_pos_y > 0 ? -1 : next_pos_y < 0 ? 1 : 0;
        set_direction(direction_x,direction_y);
    } else {
        set_direction(0,0);
    }   
    move();
    if (impaired)
        speed = speed * 2;
    return GameConfig::get_instance()->get_value<int>("CODE_NULL");
}

int Witch::calculate_next_movement(std::vector<PlayerState>& players) {
    if (this->health == 0)
        return GameConfig::get_instance()->get_value<int>("CODE_NULL");
    if(this->smoked_time > 0) {
        smoked_time--;
        state = IDLE;
        return GameConfig::get_instance()->get_value<int>("CODE_NULL");
    }
    GameConfig *config = GameConfig::get_instance();
    if (state == IDLE) {
        int x = getRandomNumber(0,config->get_value<int>("WITCH_SCREAM_CHANCE"));
        if (x == 2) {
            state = SCREAMING;
        }
    } else if (state == SCREAMING) {
        int x = getRandomNumber(0,config->get_value<int>("WITCH_SPAWN_CHANCE"));
        if (x == 2) {
            return GameConfig::get_instance()->get_value<int>("CODE_WITCH_SPAWN");
        }
    }
    return GameConfig::get_instance()->get_value<int>("CODE_NULL");
}

int Jumper::calculate_next_movement(std::vector<PlayerState>& players) {
    if(this->smoked_time > 0) {
        smoked_time--;
    }
    if (this->health == 0)
        return GameConfig::get_instance()->get_value<int>("CODE_NULL");
    if (this->state == HURT && cooldown > 0 && this->smoked_time == 0) {
        cooldown--;
    } else if (this->state == HURT && cooldown == 0) {
        this->state = IDLE;
    } else if (this->state == IDLE && this->smoked_time == 0) {
        set_objetive(players);
    } else if (this->state == JUMPING) {
        if (jump()) { //termina el salto, entra en cooldown
            GameConfig *config = GameConfig::get_instance();
            this->state = HURT;
            this->cooldown = config->get_value<int>("JUMPER_COOLDOWN");;
            this->objetive.x = -1;
            this->objetive.y = -1;
        }
    }
    return GameConfig::get_instance()->get_value<int>("CODE_NULL");
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
    if (this->smoked_time > 0)
        smoked_time--;
    if (this->health == 0)
        return GameConfig::get_instance()->get_value<int>("CODE_NULL");
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
    GameConfig *config = GameConfig::get_instance();
    if (distance < seeking_distance) {
        float next_pos_x = this_pos.x - closest_x;
        float next_pos_y = this_pos.y - closest_y;
        int direction_y = next_pos_y > 0 ? -1 : next_pos_y < 0 ? 1 : 0;
        set_direction(0,direction_y);
        if (next_pos_x > 0)
            facing_direction = LEFT;//como no se mueve en el eje y, siempre estaria mirando hacia adelante, esto es un fix
        if (abs(next_pos_y) < config->get_value<int>("VENOM_PROJECTILE_SIZE")) {
            if (cooldown == 0 && smoked_time == 0) {
                state = THROWING_GRENADE;
                cooldown = config->get_value<int>("VENOM_PROJECTILE_COOLDOWN");
                return GameConfig::get_instance()->get_value<int>("CODE_VENOM_PROJECTILE");
            } else {
                return GameConfig::get_instance()->get_value<int>("CODE_NULL");
            }
        set_direction(0,direction_y);
        }
    } else {
        set_direction(0,0);
    }   
    move();
    return GameConfig::get_instance()->get_value<int>("CODE_NULL");
}

void Jumper::attack(GameEntity * other) {
    if (state == JUMPING && health > 0) {
        PlayerState *player = (PlayerState*)other;
        player->take_damage(damage);
    }
}

void Witch::attack(GameEntity * other) {
    //nothing
}
