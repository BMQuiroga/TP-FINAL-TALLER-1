#ifndef _GAME_CONFIG_H
#define _GAME_CONFIG_H

#include <iostream>
#include <fstream>
#include <yaml-cpp/yaml.h>

class GameConfig {
public:
    static GameConfig* get_instance(const char *config_path = nullptr);
    static void release();

    template <typename T>
    T get_value(const std::string& key) const {
        return config_[key].as<T>();
    }
    template <typename T>
    void set_value(const std::string& key, const T& value) {
        config_[key] = value;
    }
    void save();

private:
    static GameConfig *instance;
    GameConfig(const char *config_path);
    ~GameConfig();

    GameConfig(const GameConfig&) = delete;
    GameConfig& operator=(const GameConfig&) = delete;

    void load();

    YAML::Node config_;
    std::string path;
};


#endif
