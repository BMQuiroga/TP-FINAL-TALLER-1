#include <iostream>
#include "client_asset_manager.h"
#include <string>
//#include <arpa/inet.h>

#define SOLDIER1 1
#define SOLDIER2 2
#define ZOMBIE 51
#define JUMPER 52
#define SPEAR 53
#define VENOM 54
#define WITCH 55
#define VENOM_PROJECTILE 101

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
#define JUMP_ 15000

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

	map.emplace(JUMPER + IDLE_, std::make_shared<Asset>("../resources/Jumper/Idle.png", 0, 96, 96, 4, renderer, 0));
	map.emplace(JUMPER + ATTACK_, std::make_shared<Asset>("../resources/Jumper/Attack_1.png", 0, 96, 96, 4, renderer, 0));
	map.emplace(JUMPER + DEAD_, std::make_shared<Asset>("../resources/Jumper/Dead.png", 0, 96, 96, 5, renderer, 0));
	map.emplace(JUMPER + EAT_, std::make_shared<Asset>("../resources/Jumper/Eating.png", 0, 96, 96, 11, renderer, 0));
	map.emplace(JUMPER + HURT_, std::make_shared<Asset>("../resources/Jumper/Hurt.png", 0, 96, 96, 5, renderer, 0));
	map.emplace(JUMPER + JUMP_, std::make_shared<Asset>("../resources/Jumper/Jump.png", 0, 96, 96, 6, renderer, 0));
	map.emplace(JUMPER + WALK_, std::make_shared<Asset>("../resources/Jumper/Walk.png", 0, 96, 96, 10, renderer, 0));
	map.emplace(JUMPER + RUN_, std::make_shared<Asset>("../resources/Jumper/Run.png", 0, 96, 96, 8, renderer, 0));

	map.emplace(SPEAR + IDLE_, std::make_shared<Asset>("../resources/Spear/Idle.png", 0, 128, 128, 7, renderer, 0));
	map.emplace(SPEAR + ATTACK_, std::make_shared<Asset>("../resources/Spear/Attack_1.png", 0, 128, 128, 4, renderer, 0));
	map.emplace(SPEAR + DEAD_, std::make_shared<Asset>("../resources/Spear/Dead.png", 0, 128, 128, 5, renderer, 0));
	map.emplace(SPEAR + FALL_, std::make_shared<Asset>("../resources/Spear/Fall.png", 0, 128, 128, 6, renderer, 0));
	map.emplace(SPEAR + HURT_, std::make_shared<Asset>("../resources/Spear/Hurt.png", 0, 128, 128, 3, renderer, 0));
	map.emplace(SPEAR + PROTECT_, std::make_shared<Asset>("../resources/Spear/Protect.png", 0, 128, 128, 2, renderer, 0));
	map.emplace(SPEAR + RUNANDATTACK_, std::make_shared<Asset>("../resources/Spear/Run+attack.png", 0, 128, 128, 5, renderer, 0));
	map.emplace(SPEAR + WALK_, std::make_shared<Asset>("../resources/Spear/Walk.png", 0, 128, 128, 7, renderer, 0));
	map.emplace(SPEAR + RUN_, std::make_shared<Asset>("../resources/Spear/Run.png", 0, 128, 128, 6, renderer, 0));

	map.emplace(VENOM + IDLE_, std::make_shared<Asset>("../resources/Venom/Idle.png", 0, 128, 128, 7, renderer, 0));
	map.emplace(VENOM + ATTACK_, std::make_shared<Asset>("../resources/Venom/Attack1.png", 0, 128, 128, 8, renderer, 0));
	map.emplace(VENOM + DEAD_, std::make_shared<Asset>("../resources/Venom/Dead.png", 0, 128, 128, 6, renderer, 0));
	map.emplace(VENOM + JUMP_, std::make_shared<Asset>("../resources/Venom/Jump.png", 0, 128, 128, 9, renderer, 0));
	map.emplace(VENOM + HURT_, std::make_shared<Asset>("../resources/Venom/Hurt.png", 0, 128, 128, 3, renderer, 0));
	map.emplace(VENOM + WALK_, std::make_shared<Asset>("../resources/Venom/Walk.png", 0, 128, 128, 6, renderer, 0));
	map.emplace(VENOM + RUN_, std::make_shared<Asset>("../resources/Venom/Run.png", 0, 128, 128, 8, renderer, 0));

	map.emplace(VENOM_PROJECTILE, std::make_shared<Asset>("../resources/Venom/Attack1a.png", 0, 64, 64, 12, renderer, 0));

	map.emplace(WITCH + IDLE_, std::make_shared<Asset>("../resources/Witch/Idle.png", 0, 96, 96, 5, renderer, 0));
	map.emplace(WITCH + ATTACK_, std::make_shared<Asset>("../resources/Witch/Attack_1.png", 0, 96, 96, 4, renderer, 0));
	map.emplace(WITCH + DEAD_, std::make_shared<Asset>("../resources/Witch/Dead.png", 0, 96, 96, 5, renderer, 0));
	map.emplace(WITCH + HURT_, std::make_shared<Asset>("../resources/Witch/Hurt.png", 0, 96, 96, 3, renderer, 0));
	map.emplace(WITCH + JUMP_, std::make_shared<Asset>("../resources/Witch/Jump.png", 0, 96, 96, 6, renderer, 0));
	map.emplace(WITCH + SCREAM_, std::make_shared<Asset>("../resources/Witch/Scream.png", 0, 96, 96, 5, renderer, 0));
	map.emplace(WITCH + WALK_, std::make_shared<Asset>("../resources/Witch/Walk.png", 0, 96, 96, 7, renderer, 0));
	map.emplace(WITCH + RUN_, std::make_shared<Asset>("../resources/Witch/Run.png", 0, 96, 96, 7, renderer, 0));

	map.emplace(ZOMBIE + IDLE_, std::make_shared<Asset>("../resources/Zombie/Idle.png", 0, 96, 96, 8, renderer, 0));
	map.emplace(ZOMBIE + ATTACK_, std::make_shared<Asset>("../resources/Zombie/Attack_1.png", 0, 96, 96, 5, renderer, 0));
	map.emplace(ZOMBIE + DEAD_, std::make_shared<Asset>("../resources/Zombie/Dead.png", 0, 96, 96, 5, renderer, 0));
	map.emplace(ZOMBIE + HURT_, std::make_shared<Asset>("../resources/Zombie/Hurt.png", 0, 96, 96, 3, renderer, 0));
	map.emplace(ZOMBIE + JUMP_, std::make_shared<Asset>("../resources/Zombie/Jump.png", 0, 96, 96, 8, renderer, 0));
	map.emplace(ZOMBIE + BITE_, std::make_shared<Asset>("../resources/Zombie/Bite.png", 0, 96, 96, 11, renderer, 0));
	map.emplace(ZOMBIE + WALK_, std::make_shared<Asset>("../resources/Zombie/Walk.png", 0, 96, 96, 8, renderer, 0));
	map.emplace(ZOMBIE + RUN_, std::make_shared<Asset>("../resources/Zombie/Run.png", 0, 96, 96, 7, renderer, 0));

	sound_map.emplace(151,SDL2pp::Music("../resources/Sound/singleshot.mp3"));
}

Asset* AssetManager::GetAsset(int code) {
    return map.at(code).get();
}

void AssetManager::play(int code, SDL2pp::Mixer & mixer) {
	mixer.PlayMusic(sound_map.at(code), 1);
}
