#include "game_config.h"

GameConfig* GameConfig::instance = nullptr;

GameConfig::GameConfig() {
    load_config();
}

GameConfig* GameConfig::get_instance() {
    if (instance == nullptr) {
        instance = new GameConfig();
    }
    return instance;
}

void GameConfig::load_config() {
    std::ifstream file("config.yml");
    if (file) {
        config_ = YAML::Load(file);
        file.close();
    } else {
        std::cerr << "Failed to open configuration file.\n";
    }
}
