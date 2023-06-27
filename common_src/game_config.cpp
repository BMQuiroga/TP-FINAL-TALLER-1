#include "game_config.h"

GameConfig* GameConfig::instance = nullptr;

GameConfig::GameConfig(const char *config_path) : path(config_path ? config_path : "config.yml") {
    load();
}

GameConfig::~GameConfig () {}

GameConfig* GameConfig::get_instance(const char *config_path) {
    if (instance == nullptr) {
        instance = new GameConfig(config_path);
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
    std::lock_guard<std::mutex> lock(mtx);
    std::ifstream file(path);
    if (file) {
        config_ = YAML::Load(file);
        file.close();
    } else {
        std::cerr << "Failed to open configuration file.\n";
    }
}

void GameConfig::save() {
    std::lock_guard<std::mutex> lock(mtx);
    std::ofstream file(path);
    if (file) {
        file << config_;
        file.close();
    } else {
        std::cerr << "Failed to open configuration file\n";
    }
}
