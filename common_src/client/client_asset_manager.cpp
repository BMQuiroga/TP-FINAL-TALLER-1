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

AssetManager::AssetManager(SDL2pp::Renderer & renderer) {
    //map.emplace(1, std::make_unique<Asset>("../resources/Soldier_1/Idle.png", 0, 128, 128, 7, renderer, 0));
    //map.emplace(2, std::make_unique<Asset>("../resources/Soldier_2/Idle.png", 0, 128, 128, 7, renderer, 0));
}

Asset* AssetManager::GetAsset(int code) {
    //return map.at(code);
}