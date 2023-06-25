#include <gtest/gtest.h>
#include "../common_src/game_config.h"

TEST(GameConfigTest, TestGetConfiguratonValue) {
    GameConfig *config = GameConfig::get_instance();
    // Expect equality.
    EXPECT_EQ(config->get_value<int>("PLAYER_HP"), 100);
}

TEST(GameConfigTest, TestSaveAndLoadConfiguraton) {
    GameConfig *config = GameConfig::get_instance();
    config->release();
    config = GameConfig::get_instance();
    // Expect equality.
    EXPECT_EQ(config->get_value<int>("PLAYER_HP"), 100);
}

TEST(GameConfigTest, TestSetAndSaveConfiguratonValue) {
    GameConfig *config = GameConfig::get_instance();
    int original = config->get_value<int>("PLAYER_HP");
    int new_ = 1000;
    config->set_value("PLAYER_HP", new_);
    config->release(); // save the configuration
    config = GameConfig::get_instance(); // load the file again
    // Expect equality.
    EXPECT_EQ(config->get_value<int>("PLAYER_HP"), new_);
    config->set_value("PLAYER_HP", original);
    config->save();
}
