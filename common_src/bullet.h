#ifndef _BULLET_H
#define _BULLET_H

#include "protocol_types.h"
#include "zombie.h"
#include <list>

class Bullet : public GameEntity {
  protected:
    uint8_t damage;
    uint8_t bullet_count;//como hay armas que disparan rafagas
    bool piercing;//hay un tipo de bala que atraviesa
    bool falloff;//perdida de daño en distancia
  public:
    Bullet(uint8_t dmg, uint8_t bc, entity_direction direc, bool piercing, Vector2D position, bool falloff);
    PlayerStateReference make_ref();
    void move();
    void attack(GameEntity *other) override;
    bool is_off_scope();
    void on_collission_detected(GameEntity *other) override;
};


#endif
