#include <gtest/gtest.h>
#include "../common_src/game_config.h"

TEST(GameConfigTest, BasicAssertions) {
    GameConfig *instance = GameConfig::get_instance();
    // Expect equality.
    // EXPECT_EQ(instance->get_value<int>("PLAYER_HP"), 100);
}