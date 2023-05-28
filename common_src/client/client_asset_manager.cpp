#include <iostream>
#include "client_asset_manager.h"
#include <string>
//#include <arpa/inet.h>

AssetManager* AssetManager::sInstance = NULL;

AssetManager* AssetManager::Instance(SDL2pp::Renderer & renderer) {
	if(sInstance == NULL)
		sInstance = new AssetManager(renderer);

	return sInstance;
}

void AssetManager::Release() {
	delete sInstance;
	sInstance = NULL;
}

//(0 idle, 1 attack, 2 dead, 3 grenade, 4 hurt 5 recharge, 6 shot, 7 walk, 8 fall, 9 run, 10 protect, 11 run+atack, 12 bite, 13 scream, 14 eating)

AssetManager::AssetManager(SDL2pp::Renderer & renderer) {
	map.emplace(-2, std::make_shared<Asset>("../resources/HPBar/EmptyBar.png", 0, 300, 300, 1, renderer, 0));
	map.emplace(-1, std::make_shared<Asset>("../resources/HPBar/FullBar.png", 0, 300, 300, 1, renderer, 0));

	map.emplace(0, std::make_shared<Asset>("../resources/backgrounds/War1/Bright/War.png", 0, 1920, 1080, 1, renderer, 0));

    map.emplace(1, std::make_shared<Asset>("../resources/Soldier_1/Idle.png", 0, 128, 128, 7, renderer, 0));
	map.emplace(1001, std::make_shared<Asset>("../resources/Soldier_1/Attack.png", 0, 128, 128, 3, renderer, 0));
	map.emplace(2001, std::make_shared<Asset>("../resources/Soldier_1/Dead.png", 0, 128, 128, 4, renderer, 0));
	map.emplace(3001, std::make_shared<Asset>("../resources/Soldier_1/Grenade.png", 0, 128, 128, 9, renderer, 0));
	map.emplace(4001, std::make_shared<Asset>("../resources/Soldier_1/Hurt.png", 0, 128, 128, 3, renderer, 0));
	map.emplace(5001, std::make_shared<Asset>("../resources/Soldier_1/Recharge.png", 0, 128, 128, 13, renderer, 0));
	map.emplace(6001, std::make_shared<Asset>("../resources/Soldier_1/Shot_1.png", 0, 128, 128, 4, renderer, 0));
	map.emplace(7001, std::make_shared<Asset>("../resources/Soldier_1/Walk.png", 0, 128, 128, 7, renderer, 0));
	map.emplace(9001, std::make_shared<Asset>("../resources/Soldier_1/Run.png", 0, 128, 128, 8, renderer, 0));

    map.emplace(2, std::make_shared<Asset>("../resources/Soldier_2/Idle.png", 0, 128, 128, 9, renderer, 0));
	map.emplace(1002, std::make_shared<Asset>("../resources/Soldier_2/Attack.png", 0, 128, 128, 4, renderer, 0));
	map.emplace(2002, std::make_shared<Asset>("../resources/Soldier_2/Dead.png", 0, 128, 128, 4, renderer, 0));
	map.emplace(3002, std::make_shared<Asset>("../resources/Soldier_2/Grenade.png", 0, 128, 128, 16, renderer, 0));
	map.emplace(4002, std::make_shared<Asset>("../resources/Soldier_2/Hurt.png", 0, 128, 128, 3, renderer, 0));
	map.emplace(5002, std::make_shared<Asset>("../resources/Soldier_2/Recharge.png", 0, 128, 128, 7, renderer, 0));
	map.emplace(6002, std::make_shared<Asset>("../resources/Soldier_2/Shot_1.png", 0, 128, 128, 4, renderer, 0));
	map.emplace(7002, std::make_shared<Asset>("../resources/Soldier_2/Walk.png", 0, 128, 128, 8, renderer, 0));
	map.emplace(9002, std::make_shared<Asset>("../resources/Soldier_2/Run.png", 0, 128, 128, 8, renderer, 0));
}

Asset* AssetManager::GetAsset(int code) {
    return map.at(code).get();
}