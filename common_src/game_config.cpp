#include "game_config.h"
#include <iostream>
#include <yaml-cpp/yaml.h>

// GameConfig::GameConfig() {
//     YAML::Node config = YAML::LoadFile("../config.yaml");
//     default_max_x = config["default_max_x"].as<int>();
//     default_max_y = config["default_max_y"].as<int>();
//     starting_hit_points = config["starting_hit_points"].as<int>();
//     gun_magazine_size = config["gun_magazine_size"].as<int>();
//     player_rect_width = config["player_rect_width"].as<int>();
//     player_rect_height = config["player_rect_height"].as<int>();
//     zombie_rect_width = config["zombie_rect_width"].as<int>();
//     zombie_rect_height = config["zombie_rect_height"].as<int>();
// }

GameConfig::GameConfig() {
    parseConfigFile("../config.yaml");
}

std::map<std::string, std::map<std::string, int>> GameConfig::parseMapNodes(
    const YAML::Node& config, 
    const std::string& node_name) {
    std::map<std::string, std::map<std::string, int>> parsed_nodes;
    const YAML::Node& nodes = config["Game"][node_name];
    for (const auto& node : nodes) {
        std::map<std::string, int> node_data;
        std::string node_name;
        for (const auto& entry : node) {
            node_name = entry.first.as<std::string>();
            for (const auto& attribute: entry.second) {
                std::string codeName = attribute.first.as<std::string>();
                int codeValue = attribute.second.as<int>();
                node_data[codeName] = codeValue;
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
                std::string codeName = attribute.first.as<std::string>();
                int codeValue = attribute.second.as<int>();
                node_data[codeName] = codeValue;
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
    defaultMaxX = config["Game"]["default_max_x"].as<int>();
    defaultMaxY = config["Game"]["default_max_y"].as<int>();
    startingHitPoints = config["Game"]["starting_hit_points"].as<int>();
    eventsPerLoop = config["Game"]["events_per_loop"].as<int>();
    gunMagazineSize = config["Game"]["gun_magazine_size"].as<int>();
    playerRectWidth = config["Game"]["player_rect_width"].as<int>();
    playerRectHeight = config["Game"]["player_rect_height"].as<int>();

    // Parse the map paths
    const YAML::Node& mapPathsNode = config["Game"]["MapPaths"];
    for (const auto& path : mapPathsNode) {
        std::string mapName = path["Map"]["name"].as<std::string>();
        std::string mapPath = path["Map"]["path"].as<std::string>();
        mapPaths.push_back(std::make_pair(mapName, mapPath));
    }

    // Parse the weapons
    weapons = parseMapNodes(config, "Weapons");
    // const YAML::Node& weaponsNode = config["Game"]["Weapons"];
    // for (const auto& weapon : weaponsNode) {
    //     std::map<std::string, std::int> weaponData;
    //     weaponData["name"] = weaponNode.first.as<std::string>();
    //     weaponData["damage"] = weapon["Weapon"]["damage"].as<std::string>();
    //     weaponData["bulletcount"] = weapon["Weapon"]["bulletcount"].as<std::string>();
    //     weaponData["magazine"] = weapon["Weapon"]["magazine"].as<std::string>();
    //     weaponData["shootdelay"] = weapon["Weapon"]["shootdelay"].as<std::string>();
    //     weaponData["reloadelay"] = weapon["Weapon"]["reloadelay"].as<std::string>();
    //     weaponData["grenadedelay"] = weapon["Weapon"]["grenadedelay"].as<std::string>();
    //     weapons.push_back(weaponData);
    // }

    // // Parse the enemies
    enemies = parseToVector(config, "Enemies");
    // const YAML::Node& enemiesNode = config["Game"]["Enemies"];
    // for (const auto& enemy : enemiesNode) {
    //     std::map<std::string, std::string> enemyData;
    //     enemyData["name"] = enemy["Enemy"]["name"].as<std::string>();
    //     enemyData["speed"] = enemy["Enemy"]["speed"].as<std::string>();
    //     enemyData["rect_width"] = enemy["Enemy"]["rect_width"].as<std::string>();
    //     // Add other enemy properties as needed
    //     enemies.push_back(enemyData);
    // }

    // // Parse the soldiers
    soldiers = parseMapNodes(config, "Soldiers");
    // const YAML::Node& soldiersNode = config["Game"]["Soldiers"];
    // for (const auto& soldier : soldiersNode) {
    //     std::map<std::string, int> soldier_data;
    //     std::string soldierName = soldier.first.as<std::string>();
    //     for (const auto& entry : soldier) {
    //         std::string codeName = entry.first.as<std::string>();
    //         int codeValue = entry.second.as<int>();
    //         soldier_data[codeName] = codeValue;
    //     }
    //     soldiers[soldierName] = soldier_data;
    // }

    // // Parse the math codes
    const YAML::Node& mathNode = config["Game"]["Math"];
    for (const auto& entry : mathNode) {
        std::string codeName = entry.first.as<std::string>();
        int codeValue = entry.second.as<int>();
        mathCodes[codeName] = codeValue;
    }

    // Parse the gameloop values
    const YAML::Node& gameloopNode = config["Game"]["Gameloop"];
    for (const auto& entry : gameloopNode) {
        std::string codeName = entry.first.as<std::string>();
        int codeValue = entry.second.as<int>();
        gameloop_values[codeName] = codeValue;
    }

}

GameConfig* GameConfig::sInstance = nullptr;

GameConfig* GameConfig::get_instance() {
	if(sInstance == nullptr)
		sInstance = new GameConfig();

	return sInstance;
}


// void GameConfig::release() {
// 	delete sInstance;
// 	sInstance = NULL;
// }

int GameConfig::getPort() const {
    return port;
}

int GameConfig::getDefaultMaxX() const {
    return defaultMaxX;
}

int GameConfig::getDefaultMaxY() const {
    return defaultMaxY;
}

int GameConfig::getStartingHitPoints() const {
    return startingHitPoints;
}

int GameConfig::getEventsPerLoop() const {
    return eventsPerLoop;
}

int GameConfig::getGunMagazineSize() const {
    return gunMagazineSize;
}

int GameConfig::getPlayerRectWidth() const {
    return playerRectWidth;
}

int GameConfig::getPlayerRectHeight() const {
    return playerRectHeight;
}

std::map<std::string, int> GameConfig::get_gameloop_values() const {
    return gameloop_values;
}

std::vector<std::pair<std::string, std::string>> GameConfig::getMapPaths() const {
    return mapPaths;
}

std::vector<std::map<std::string, int>> GameConfig::getEnemies() const {
    return enemies;
}

std::map<std::string, std::map<std::string, int>> GameConfig::getSoldiers() const {
    return soldiers;
}

std::map<std::string, int> GameConfig::get_weapon(const std::string &weapon_name)
{
    return weapons[weapon_name];
}

std::map<std::string, int> GameConfig::get_enemy(int enemy)
{
    return enemies[enemy];
}
