#include <gtest/gtest.h>
#include "../common_src/game_config.h"

TEST(GameConfigTest, TestGetConfiguratonValue) {
    GameConfig *instance = GameConfig::get_instance();
    // Expect equality.
    EXPECT_EQ(instance->get_value<int>("PLAYER_HP"), 100);
}

TEST(GameConfigTest, TestSaveConfiguratonValue) {
    GameConfig *instance = GameConfig::get_instance();
    instance->release();
    instance = GameConfig::get_instance();
    // Expect equality.
    EXPECT_EQ(instance->get_value<int>("PLAYER_HP"), 100);
}

TEST(GameConfigTest, TestSetAndSaveConfiguratonValue) {
    GameConfig *instance = GameConfig::get_instance();
    int original = instance->get_value<int>("PLAYER_HP");
    int new_ = 1000;
    instance->set_value("PLAYER_HP", new_);
    instance->release(); // save the configuration
    instance = GameConfig::get_instance(); // load the file again
    // Expect equality.
    EXPECT_EQ(instance->get_value<int>("PLAYER_HP"), new_);
    instance->set_value("PLAYER_HP", original);
}
