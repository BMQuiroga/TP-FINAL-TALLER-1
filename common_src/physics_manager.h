#ifndef _PHYSICS_MANAGER_H
#define _PHYSICS_MANAGER_H

#include <bitset>
#include "game_entity.h"
#include <vector>

class GameEntity;

enum class CollisionLayer {
    Friendly,
    FriendlyProjectile,
    Hostile,
    HostileProjectile,
    MaxLayers,
};

enum class CollisionFlag {
    None = 0x00,
    Friendly = 0x01,
    FriendlyProjectile = 0x02,
    Hostile = 0x04,
    HostileProjectile = 0x08
};

class PhysicsManager {
  private:
    static PhysicsManager *instance;
    std::vector<GameEntity*> collision_layers[(uint32_t)CollisionLayer::MaxLayers];
    std::bitset<(uint32_t)CollisionLayer::MaxLayers> layer_masks[(uint32_t)CollisionLayer::MaxLayers];
    uint32_t last_id;
  public:
    static PhysicsManager *get_instance();
    static void release();
    void set_layer_collision_mask(CollisionLayer layer, CollisionFlag flag);
    uint32_t register_entity(GameEntity *entity, CollisionLayer layer);
    void unregister_entity(uint32_t id);
    void update();
  private:
    PhysicsManager();
    ~PhysicsManager();
};

inline CollisionFlag operator|(CollisionFlag a, CollisionFlag b) {
    return (CollisionFlag) ((int)a | (int)b);
}

inline CollisionFlag operator&(CollisionFlag a, CollisionFlag b) {
    return (CollisionFlag) ((int)a & (int)b);
}

#endif
