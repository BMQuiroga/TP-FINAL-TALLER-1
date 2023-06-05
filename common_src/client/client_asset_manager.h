#ifndef CLIENT_ASSET_MANAGER_H_
#define CLIENT_ASSET_MANAGER_H_

#include "../socket.h"
#include "../protocol.h"
#include <string>
#include <memory>
#include "client_asset.h"
#include <map>


class AssetManager {
    private:
    static AssetManager* sInstance;
    std::map<int,std::shared_ptr<Asset>> map;
    std::map<int,SDL2pp::Music> sound_map;

    public:
    static AssetManager* Instance(SDL2pp::Renderer & renderer);

    static void Release();

    Asset* GetAsset(int code);

    void play(int code, SDL2pp::Mixer & mixer);

    explicit AssetManager(SDL2pp::Renderer & renderer);
};

#endif
