#ifndef CLIENT_ASSET_MANAGER_H_
#define CLIENT_ASSET_MANAGER_H_

#include "../socket.h"
#include "../protocol.h"
#include <string>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/Renderer.hh>
#include "client_asset.h"
#include <map>


class AssetManager {
    private:

    static AssetManager* sInstance;
    std::map<int,std::shared_ptr<Asset>> map;

    public:

    static AssetManager* Instance(SDL2pp::Renderer & renderer);

    static void Release();

    Asset* GetAsset(int code);

    AssetManager(SDL2pp::Renderer & renderer);
};

#endif
