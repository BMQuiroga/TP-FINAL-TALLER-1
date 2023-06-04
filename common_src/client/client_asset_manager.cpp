#include <iostream>
#include "client_asset_manager.h"
#include <string>
//#include <arpa/inet.h>

#define SOLDIER1 1
#define SOLDIER2 2
#define BACKGROUND 0

#define IDLE_ 0
#define ATTACK_ 1000
#define DEAD_ 2000
#define GRENADE_ 3000
#define HURT_ 4000
#define RECHARGE_ 5000
#define SHOT_ 6000
#define WALK_ 7000
#define FALL_ 8000
#define RUN_ 9000
#define PROTECT_ 10000
#define RUNANDATTACK_ 11000
#define BITE_ 12000
#define SCREAM_ 13000
#define EAT_ 14000

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
	//map.emplace(-2, Asset::Create("../resources/HPBar/EmptyBar.png", 0, 300, 300, 1, renderer, 0));
	map.emplace(-1, std::make_shared<Asset>("../resources/HPBar/FullBar.png", 0, 300, 300, 1, renderer, 0));

	map.emplace(BACKGROUND, std::make_shared<Asset>("../resources/backgrounds/War1/Bright/War.png", 0, 1920, 1080, 1, renderer, 0));

    map.emplace(SOLDIER1 + IDLE_, std::make_shared<Asset>("../resources/Soldier_1/Idle.png", 0, 128, 128, 7, renderer, 0));
	map.emplace(SOLDIER1 + ATTACK_, std::make_shared<Asset>("../resources/Soldier_1/Attack.png", 0, 128, 128, 3, renderer, 0));
	map.emplace(SOLDIER1 + DEAD_, std::make_shared<Asset>("../resources/Soldier_1/Dead.png", 0, 128, 128, 4, renderer, 0));
	map.emplace(SOLDIER1 + GRENADE_, std::make_shared<Asset>("../resources/Soldier_1/Grenade.png", 0, 128, 128, 9, renderer, 0));
	map.emplace(SOLDIER1 + HURT_, std::make_shared<Asset>("../resources/Soldier_1/Hurt.png", 0, 128, 128, 3, renderer, 0));
	map.emplace(SOLDIER1 + RECHARGE_, std::make_shared<Asset>("../resources/Soldier_1/Recharge.png", 0, 128, 128, 13, renderer, 0));
	map.emplace(SOLDIER1 + SHOT_, std::make_shared<Asset>("../resources/Soldier_1/Shot_1.png", 0, 128, 128, 4, renderer, 0));
	map.emplace(SOLDIER1 + WALK_, std::make_shared<Asset>("../resources/Soldier_1/Walk.png", 0, 128, 128, 7, renderer, 0));
	map.emplace(SOLDIER1 + RUN_, std::make_shared<Asset>("../resources/Soldier_1/Run.png", 0, 128, 128, 8, renderer, 0));

	map.emplace(SOLDIER2 + IDLE_, std::make_shared<Asset>("../resources/Soldier_2/Idle.png", 0, 128, 128, 9, renderer, 0));
	map.emplace(SOLDIER2 + ATTACK_, std::make_shared<Asset>("../resources/Soldier_2/Attack.png", 0, 128, 128, 4, renderer, 0));
	map.emplace(SOLDIER2 + DEAD_, std::make_shared<Asset>("../resources/Soldier_2/Dead.png", 0, 128, 128, 4, renderer, 0));
	map.emplace(SOLDIER2 + GRENADE_, std::make_shared<Asset>("../resources/Soldier_2/Grenade.png", 0, 128, 128, 16, renderer, 0));
	map.emplace(SOLDIER2 + HURT_, std::make_shared<Asset>("../resources/Soldier_2/Hurt.png", 0, 128, 128, 3, renderer, 0));
	map.emplace(SOLDIER2 + RECHARGE_, std::make_shared<Asset>("../resources/Soldier_2/Recharge.png", 0, 128, 128, 7, renderer, 0));
	map.emplace(SOLDIER2 + SHOT_, std::make_shared<Asset>("../resources/Soldier_2/Shot_1.png", 0, 128, 128, 4, renderer, 0));
	map.emplace(SOLDIER2 + WALK_, std::make_shared<Asset>("../resources/Soldier_2/Walk.png", 0, 128, 128, 8, renderer, 0));
	map.emplace(SOLDIER2 + RUN_, std::make_shared<Asset>("../resources/Soldier_2/Run.png", 0, 128, 128, 8, renderer, 0));

	sound_map.emplace(151,SDL2pp::Mixer::Music("../resources/Sound/singleshot.mp3"));
}

Asset* AssetManager::GetAsset(int code) {
    return map.at(code).get();
}

void AssetManager::play(int code, SDL2pp::Mixer & mixer) {
	mixer.PlayChannel(-1, sound_map.at(code), 0);
}