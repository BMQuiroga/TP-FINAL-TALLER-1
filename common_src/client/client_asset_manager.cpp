#include <iostream>
#include "client_asset_manager.h"
#include <string>
#include "../game_config.h"
//#include <arpa/inet.h>

#define SOLDIER1 1
#define SOLDIER2 2
#define SOLDIER3 3
#define ZOMBIE 51
#define JUMPER 52
#define SPEAR 53
#define VENOM 54
#define WITCH 55
#define VENOM_PROJECTILE 101
#define HE 102
#define SMOKE 103

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

#define ROAD -8
#define REVIVE_SCREEN -7
#define WIN_SCREEN -6
#define DEATH_SCREEN -5
#define BULLET_MODEL -4
#define HEART_MODEL -3
#define EMPTY_HPBAR -2
#define FULL_HPBAR -1

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

AssetManager::AssetManager(SDL2pp::Renderer & renderer) :
	/*default_font("../resources/Fonts/ARIAL.TTF", 30),*/ default_color(50,50,50,255), white_color(255,255,255,255)
	{
	map.emplace(ROAD, std::make_shared<Asset>("../resources/backgrounds/War1/Bright/3road.png", 0, 1920, 1080, 1, renderer, 1));
	map.emplace(REVIVE_SCREEN, std::make_shared<Asset>("../resources/HPBar/ReviveScreen.png", 0, 1920, 1080, 1, renderer, 1));
	map.emplace(WIN_SCREEN, std::make_shared<Asset>("../resources/HPBar/WinScreen.png", 0, 1920, 1080, 1, renderer, 1));
	map.emplace(DEATH_SCREEN, std::make_shared<Asset>("../resources/HPBar/DeathScreen.png", 0, 1920, 1080, 1, renderer, 1));
	map.emplace(BULLET_MODEL, std::make_shared<Asset>("../resources/HPBar/bullet.png", 0, 300, 300, 1, renderer, 0));
	map.emplace(HEART_MODEL, std::make_shared<Asset>("../resources/HPBar/heart.png", 0, 300, 300, 1, renderer, 0));
	map.emplace(EMPTY_HPBAR, std::make_shared<Asset>("../resources/HPBar/EmptyBar.png", 0, 300, 300, 1, renderer, 0));
	map.emplace(FULL_HPBAR, std::make_shared<Asset>("../resources/HPBar/FullBar.png", 0, 300, 300, 1, renderer, 0));

	map.emplace(BACKGROUND, std::make_shared<Asset>("../resources/backgrounds/War1/Bright/War.png", 0, 1920, 1080, 1, renderer, 1));

    map.emplace(SOLDIER1 + IDLE_, std::make_shared<Asset>("../resources/Soldier_1/Idle.png", 0, 128, 128, 7, renderer, 0));
	map.emplace(SOLDIER1 + ATTACK_, std::make_shared<Asset>("../resources/Soldier_1/Attack.png", 0, 128, 128, 3, renderer, 0));
	map.emplace(SOLDIER1 + DEAD_, std::make_shared<Asset>("../resources/Soldier_1/Dead.png", 0, 128, 128, 4, renderer, 0));
	map.emplace(SOLDIER1 + GRENADE_, std::make_shared<Asset>("../resources/Soldier_1/Grenade.png", 0, 128, 128, 9, renderer, 0));
	map.emplace(SOLDIER1 + RECHARGE_, std::make_shared<Asset>("../resources/Soldier_1/Recharge.png", 0, 128, 128, 13, renderer, 0));
	//map.emplace(SOLDIER1 + SHOT_, std::make_shared<Asset>("../resources/Soldier_1/Shot_1.png", 0, 128, 128, 4, renderer, 0));
	map.emplace(SOLDIER1 + WALK_, std::make_shared<Asset>("../resources/Soldier_1/Walk.png", 0, 128, 128, 7, renderer, 0));
	//map.emplace(SOLDIER1 + RUN_, std::make_shared<Asset>("../resources/Soldier_1/Run.png", 0, 128, 128, 8, renderer, 0));

	map.emplace(SOLDIER2 + IDLE_, std::make_shared<Asset>("../resources/Soldier_2/Idle.png", 0, 128, 128, 9, renderer, 0));
	map.emplace(SOLDIER2 + ATTACK_, std::make_shared<Asset>("../resources/Soldier_2/Attack.png", 0, 128, 128, 4, renderer, 0));
	map.emplace(SOLDIER2 + DEAD_, std::make_shared<Asset>("../resources/Soldier_2/Dead.png", 0, 128, 128, 4, renderer, 0));
	map.emplace(SOLDIER2 + GRENADE_, std::make_shared<Asset>("../resources/Soldier_2/Grenade.png", 0, 128, 128, 16, renderer, 0));
	map.emplace(SOLDIER2 + RECHARGE_, std::make_shared<Asset>("../resources/Soldier_2/Recharge.png", 0, 128, 128, 7, renderer, 0));
	//map.emplace(SOLDIER2 + SHOT_, std::make_shared<Asset>("../resources/Soldier_2/Shot_1.png", 0, 128, 128, 4, renderer, 0));
	map.emplace(SOLDIER2 + WALK_, std::make_shared<Asset>("../resources/Soldier_2/Walk.png", 0, 128, 128, 8, renderer, 0));
	//map.emplace(SOLDIER2 + RUN_, std::make_shared<Asset>("../resources/Soldier_2/Run.png", 0, 128, 128, 8, renderer, 0));

	map.emplace(SOLDIER3 + IDLE_, std::make_shared<Asset>("../resources/Soldier_3/Idle.png", 0, 128, 128, 7, renderer, 0));
	map.emplace(SOLDIER3 + ATTACK_, std::make_shared<Asset>("../resources/Soldier_3/Attack.png", 0, 128, 128, 5, renderer, 0));
	map.emplace(SOLDIER3 + DEAD_, std::make_shared<Asset>("../resources/Soldier_3/Dead.png", 0, 128, 128, 5, renderer, 0));
	map.emplace(SOLDIER3 + GRENADE_, std::make_shared<Asset>("../resources/Soldier_3/Grenade.png", 0, 128, 128, 8, renderer, 0));
	map.emplace(SOLDIER3 + RECHARGE_, std::make_shared<Asset>("../resources/Soldier_3/Recharge.png", 0, 128, 128, 8, renderer, 0));
	//map.emplace(SOLDIER3 + SHOT_, std::make_shared<Asset>("../resources/Soldier_3/Shot_1.png", 0, 128, 128, 4, renderer, 0));
	map.emplace(SOLDIER3 + WALK_, std::make_shared<Asset>("../resources/Soldier_3/Walk.png", 0, 128, 128, 8, renderer, 0));
	//map.emplace(SOLDIER3 + RUN_, std::make_shared<Asset>("../resources/Soldier_3/Run.png", 0, 128, 128, 6, renderer, 0));

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
	map.emplace(VENOM + ATTACK_, std::make_shared<Asset>("../resources/Venom/Attack2.png", 0, 128, 128, 14, renderer, 0));
	map.emplace(VENOM + GRENADE_, std::make_shared<Asset>("../resources/Venom/Attack1.png", 0, 128, 128, 8, renderer, 0));
	map.emplace(VENOM + DEAD_, std::make_shared<Asset>("../resources/Venom/Dead.png", 0, 128, 128, 6, renderer, 0));
	map.emplace(VENOM + JUMP_, std::make_shared<Asset>("../resources/Venom/Jump.png", 0, 128, 128, 9, renderer, 0));
	map.emplace(VENOM + HURT_, std::make_shared<Asset>("../resources/Venom/Hurt.png", 0, 128, 128, 3, renderer, 0));
	map.emplace(VENOM + WALK_, std::make_shared<Asset>("../resources/Venom/Walk.png", 0, 128, 128, 6, renderer, 0));
	map.emplace(VENOM + RUN_, std::make_shared<Asset>("../resources/Venom/Run.png", 0, 128, 128, 8, renderer, 0));

	map.emplace(VENOM_PROJECTILE, std::make_shared<Asset>("../resources/Venom/Attack1a.png", 0, 64, 64, 3, renderer, 0));

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

	map.emplace(HE + DEAD_, std::make_shared<Asset>("../resources/Soldier_1/Explosion.png", 0, 128, 128, 9, renderer, 0));
	map.emplace(SMOKE + DEAD_, std::make_shared<Asset>("../resources/Soldier_3/Smoke.png", 0, 128, 128, 22, renderer, 0));
	//DEAD_ es es standar para la animacion que se queda trabada en el ultimo frame en vez de loopear

	sound_map.emplace(0,SDL2pp::Music("../resources/Sound/Music.mp3"));

	sound_map.emplace(152,SDL2pp::Music("../resources/Sound/DeathScreen.mp3"));
	sound_map.emplace(153,SDL2pp::Music("../resources/Sound/VictoryScreen.mp3"));
	sound_map.emplace(154,SDL2pp::Music("../resources/Sound/IDF.mp3"));
	sound_map.emplace(155,SDL2pp::Music("../resources/Sound/P90.mp3"));
	sound_map.emplace(156,SDL2pp::Music("../resources/Sound/Scout.mp3"));

	sound_map.emplace(157,SDL2pp::Music("../resources/Sound/Bounce.mp3"));
	sound_map.emplace(158,SDL2pp::Music("../resources/Sound/HE.mp3"));
	sound_map.emplace(159,SDL2pp::Music("../resources/Sound/Smoke.mp3"));

	sound_map.emplace(160,SDL2pp::Music("../resources/Sound/Reload.mp3"));

	sound_map.emplace(161,SDL2pp::Music("../resources/Sound/Acid.mp3"));
	sound_map.emplace(162,SDL2pp::Music("../resources/Sound/AirStrike.mp3"));
}

Asset* AssetManager::GetAsset(int code) {
	//std::cout << "TRIED TO RENDER: " << std::to_string(code) << std::endl;
    return map.at(code).get();
}

void AssetManager::play(int code, SDL2pp::Mixer & mixer) {
	mixer.PlayMusic(sound_map.at(code), 1);
}

SDL2pp::Color* AssetManager::get_default_color() {
	SDL2pp::Color* p = &default_color;
	return p;
}

SDL2pp::Color* AssetManager::get_white_color() {
	SDL2pp::Color* p = &white_color;
	return p;
}

void AssetManager::play_music(SDL2pp::Mixer & mixer) {
	//mixer.PlayMusic(sound_map.at(0), -1);
}
/*
SDL2pp::Font* AssetManager::get_default_font() {
	SDL2pp::Font* p = &default_font;
	return p;
}*/
