#include <gtest/gtest.h>
#include "../common_src/physics_manager.h"
#include "../common_src/player_state.h"
#include "../common_src/zombie.h"


TEST(CollisionTests, TestNoCollisionDetected) {
    PhysicsManager physics;
    physics.set_layer_collision_mask(
        CollisionLayer::Friendly,
        CollisionFlag::Hostile | CollisionFlag::HostileProjectile
    );
    physics.set_layer_collision_mask(
        CollisionLayer::Hostile,
        CollisionFlag::Friendly
    );
    PlayerState player("test player", 0, 1, &physics);
    CommonZombie zombie("test zombie", Vector2D(player.get_location().x + 100, player.get_location().y), &physics);
    
    // test that all entities registered correctly
    ASSERT_EQ(physics.get_collision_layer(CollisionLayer::Friendly).size(), 1);
    ASSERT_EQ(physics.get_collision_layer(CollisionLayer::Friendly)[0], &player);
    ASSERT_EQ(physics.get_collision_layer(CollisionLayer::Hostile).size(), 1);
    ASSERT_EQ(physics.get_collision_layer(CollisionLayer::Hostile)[0], &zombie);

    int original_health = player.get_hit_points();
    physics.update();

    // player should not have been hit, so health should be the same as before
    ASSERT_EQ(original_health, player.get_hit_points());
}

TEST(CollisionTests, TestZombieHitPlayer) {
    PhysicsManager physics;
    physics.set_layer_collision_mask(
        CollisionLayer::Friendly,
        CollisionFlag::Hostile | CollisionFlag::HostileProjectile
    );
    physics.set_layer_collision_mask(
        CollisionLayer::Hostile,
        CollisionFlag::Friendly
    );
    PlayerState player("test player", 0, 1, &physics);
    CommonZombie zombie("test zombie", Vector2D(player.get_location().x + 5, player.get_location().y), &physics);
    
    // test that all entities registered correctly
    ASSERT_EQ(physics.get_collision_layer(CollisionLayer::Friendly).size(), 1);
    ASSERT_EQ(physics.get_collision_layer(CollisionLayer::Friendly)[0], &player);
    ASSERT_EQ(physics.get_collision_layer(CollisionLayer::Hostile).size(), 1);
    ASSERT_EQ(physics.get_collision_layer(CollisionLayer::Hostile)[0], &zombie);

    int original_health = player.get_hit_points();
    physics.update();

    // player should have been hit, so health should be lower
    ASSERT_GT(original_health, player.get_hit_points());
}

TEST(CollisionTests, TestBulletHitZombie) {
    PhysicsManager physics;
    physics.set_layer_collision_mask(
        CollisionLayer::FriendlyProjectile,
        CollisionFlag::Hostile
    );
    physics.set_layer_collision_mask(
        CollisionLayer::Hostile,
        CollisionFlag::FriendlyProjectile
    );
    CommonZombie zombie("test zombie", Vector2D(20, 20), &physics);
    Arma1 arma;
    Bullet bullet(
        1,
        1,
        LEFT,
        false,
        Vector2D(zombie.get_location().x + 20, zombie.get_location().y),
        true,
        &physics
    );
    
    // test that all entities registered correctly
    ASSERT_EQ(physics.get_collision_layer(CollisionLayer::FriendlyProjectile).size(), 1);
    ASSERT_EQ(physics.get_collision_layer(CollisionLayer::FriendlyProjectile)[0], &bullet);
    ASSERT_EQ(physics.get_collision_layer(CollisionLayer::Hostile).size(), 1);
    ASSERT_EQ(physics.get_collision_layer(CollisionLayer::Hostile)[0], &zombie);

    int original_health = zombie.get_health();
    physics.update();

    // zombie should have been hit, so health should be lower
    ASSERT_GT(original_health, zombie.get_health());
}
