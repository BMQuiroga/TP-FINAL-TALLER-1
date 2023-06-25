#include "game_config.h"
#include <iostream>
#include <yaml-cpp/yaml.h>

GameConfig::GameConfig() {
    parseConfigFile("../config.yaml");
}

std::map<std::string, int> GameConfig::parseNode(const YAML::Node &config, const std::string &node_name)
{
    std::map<std::string, int> parsed_node;
    const YAML::Node& node = config["Game"][node_name];
    for (const auto& entry : node) {
        std::string code_name = entry.first.as<std::string>();
        int code_value = entry.second.as<int>();
        parsed_node[code_name] = code_value;
    }
    return parsed_node;
}

std::map<std::string, std::map<std::string, int>> GameConfig::parseMapNodes(
    const YAML::Node &config,
    const std::string &node_name)
{
    std::map<std::string, std::map<std::string, int>> parsed_nodes;
    const YAML::Node& nodes = config["Game"][node_name];
    for (const auto& node : nodes) {
        std::map<std::string, int> node_data;
        std::string node_name;
        for (const auto& entry : node) {
            node_name = entry.first.as<std::string>();
            for (const auto& attribute: entry.second) {
                std::string code_name = attribute.first.as<std::string>();
                int code_value = attribute.second.as<int>();
                node_data[code_name] = code_value;
            }
        }
        parsed_nodes[node_name] = node_data;
    }
    return parsed_nodes;
}

std::vector<std::map<std::string, int>> GameConfig::parseToVector(
    const YAML::Node& config, 
    const std::string& node_name) {
    std::vector<std::map<std::string, int>> parsed_nodes;
    const YAML::Node& nodes = config["Game"][node_name];
    for (const auto& node : nodes) {
        std::map<std::string, int> node_data;
        for (const auto& entry : node) {
            for (const auto& attribute: entry.second) {
                std::string code_name = attribute.first.as<std::string>();
                int code_value = attribute.second.as<int>();
                node_data[code_name] = code_value;
            }
        }
        parsed_nodes.push_back(node_data);
    }
    return parsed_nodes;
}

void GameConfig::parseConfigFile(const std::string& filename) {
    YAML::Node config = YAML::LoadFile(filename);

    // Parse the configuration data and store it in member variables
    port = config["Game"]["port"].as<int>();
    max_x = config["Game"]["default_max_x"].as<int>();
    max_y = config["Game"]["default_max_y"].as<int>();
    events_per_loop = config["Game"]["events_per_loop"].as<int>();

    // Parse the map paths
    const YAML::Node& mapPathsNode = config["Game"]["MapPaths"];
    for (const auto& path : mapPathsNode) {
        std::string mapName = path["Map"]["name"].as<std::string>();
        std::string mapPath = path["Map"]["path"].as<std::string>();
        mapPaths.push_back(std::make_pair(mapName, mapPath));
    }

    // Parse the weapons
    weapons = parseMapNodes(config, "Weapons");

    // Parse the enemies
    enemies = parseToVector(config, "Enemies");

    // Parse the soldiers
    soldiers = parseMapNodes(config, "Soldiers");

    // Parse player info
    playerstate = parseNode(config, "Playerstate");

    // // Parse the math codes
    mathCodes = parseNode(config, "Math");
    // const YAML::Node& mathNode = config["Game"]["Math"];
    // for (const auto& entry : mathNode) {
    //     std::string code_name = entry.first.as<std::string>();
    //     int code_value = entry.second.as<int>();
    //     mathCodes[code_name] = code_value;
    // }

    // Parse the gameloop values
    gameloop_values = parseNode(config, "Gameloop");
    // const YAML::Node& gameloopNode = config["Game"]["Gameloop"];
    // for (const auto& entry : gameloopNode) {
    //     std::string code_name = entry.first.as<std::string>();
    //     int code_value = entry.second.as<int>();
    //     gameloop_values[code_name] = code_value;
    // }

}

void GameConfig::release() {
	delete instance;
	instance = nullptr;
}

int GameConfig::getPort() const {
    return port;
}

int GameConfig::get_max_x() const {
    return max_x;
}

int GameConfig::get_max_y() const {
    return max_y;
}

int GameConfig::get_events_per_loop() const {
    return events_per_loop;
}

std::map<std::string, int> GameConfig::get_gameloop_values() const {
    return gameloop_values;
}

std::vector<std::pair<std::string, std::string>> GameConfig::getMapPaths() const {
    return mapPaths;
}


std::map<std::string, std::map<std::string, int>> GameConfig::getSoldiers() const {
    return soldiers;
}

std::map<std::string, int> GameConfig::get_weapon(const std::string &weapon_name)
{
    return weapons[weapon_name];
}

std::map<std::string, int> GameConfig::get_playerstate() const {
    return playerstate;
}

std::map<std::string, int> GameConfig::get_enemy(int enemy)
{
    return enemies[enemy];
}

GameConfig* GameConfig::instance = nullptr;

GameConfig* GameConfig::get_instance() {
    if (instance == nullptr) {
        instance = new GameConfig();
    }
    return instance;
}
