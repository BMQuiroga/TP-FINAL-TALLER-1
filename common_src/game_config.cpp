#include "game_config.h"

GameConfig* GameConfig::instance = nullptr;

GameConfig::GameConfig() {
    load();
}

GameConfig::~GameConfig () {}

GameConfig* GameConfig::get_instance() {
    if (instance == nullptr) {
        instance = new GameConfig();
    }
    return instance;
}

void GameConfig::release() {
    if (instance) {
        instance->save();
        delete instance;
        instance = nullptr;
    }
}

void GameConfig::load() {
    std::ifstream file("config.yml");
    if (file) {
        config_ = YAML::Load(file);
        file.close();
    } else {
        std::cerr << "Failed to open configuration file.\n";
    }
}

void GameConfig::save() {
    std::ofstream file("config.yml");
    if (file) {
        file << config_;
        file.close();
    } else {
        std::cerr << "Failed to open configuration file\n";
    }
}
