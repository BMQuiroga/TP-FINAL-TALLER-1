#include <iostream>
#include "client_asset_manager.h"
#include <string>
//#include <arpa/inet.h>

AssetManager* AssetManager::Instance() {
	if(sInstance == NULL)
		sInstance = new AssetManager();

	return sInstance;
}

void AssetManager::Release() {
	delete sInstance;
	sInstance = NULL;
}

AssetManager::AssetManager() {

}

SDL_Texture* AssetManager::GetTexture(std::string path) {
	return map[path];
}