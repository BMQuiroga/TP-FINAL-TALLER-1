#include "physics_manager.h"

PhysicsManager* PhysicsManager::instance = nullptr;

PhysicsManager* PhysicsManager::get_instance() {
    if (instance == nullptr)
        instance = new PhysicsManager();
    return instance;
}

void PhysicsManager::release() {
    delete instance;
    instance = nullptr;
}

void PhysicsManager::set_layer_collision_mask(
    CollisionLayer layer, CollisionFlag flag
) {
    layer_masks[(uint32_t)layer] = std::bitset<(uint32_t)CollisionLayer::MaxLayers>((uint32_t)flag);
}

uint32_t PhysicsManager::register_entity(GameEntity *entity, CollisionLayer layer) {
    collision_layers[(uint32_t)layer].push_back(entity);
    last_id++;
    return last_id;
}

void PhysicsManager::unregister_entity(uint32_t id) {
    bool found = false;
    for (uint32_t i = 0; i < (uint32_t)CollisionLayer::MaxLayers && !found; i++) {
        for (uint32_t j = 0; j < (uint32_t)collision_layers[i].size() && !found; j++) {
            if (collision_layers[i][j]->get_physics_id() == id) {
                collision_layers[i].erase(collision_layers[i].begin() + j);
                found = true;
            }
        }
    }
}

PhysicsManager::PhysicsManager() {
    last_id = 0;
    for (int i = 0; i < (int)CollisionLayer::MaxLayers; i++) {
        layer_masks[i] = std::bitset<(uint32_t)CollisionLayer::MaxLayers>((uint32_t)CollisionFlag::None);
    }
}

PhysicsManager::~PhysicsManager() {
    for (int i = 0; i < (int)CollisionLayer::MaxLayers; i++) {
        collision_layers[i].clear();
    }
}

void PhysicsManager::update() {
    for (int i = 0; i < (int)CollisionLayer::MaxLayers; i++) {
        for (int j = 0; j < (int)CollisionLayer::MaxLayers; j++) {
            /*
            layer masks:
            0 (Friendly): 1100 (Hostile | Hostile Projectile)
            1 (FriendlyProjectile): 1100 (Hostile | Hostile Projectile)
            2 (Hostile): 0011 (Friendly | FriendlyProjectile)
            3 (Hostile Projectile): 0010 (Friendly)
            i = 0, j = 0
            1100.test(0) -> true
            i = 0, j = 1
            1100.test(1) -> true
            i = 0, j = 2
            1100.test(2) -> false
            i = 0, j = 3
            1100.test(3) -> false
            i = 1, j = 0
            1100.test(0) -> true
            */
            if (i <= j && layer_masks[i].test(j)) {
                for (int k = 0; k < (int)collision_layers[i].size(); k++) {
                    for (int l = 0; l < (int)collision_layers[j].size(); l++) {
                       if (collision_layers[i][k]->check_collision(collision_layers[j][l])) {
                            collision_layers[i][k]->on_collission_detected(collision_layers[j][l]);
                            collision_layers[j][l]->on_collission_detected(collision_layers[i][k]);
                       }
                    }
                }
            }
        }
    }
}