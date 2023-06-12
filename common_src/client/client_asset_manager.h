#ifndef CLIENT_ASSET_MANAGER_H_
#define CLIENT_ASSET_MANAGER_H_
#define ZOMBIE 51

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

    //constructor
    explicit AssetManager(SDL2pp::Renderer & renderer);

    public:
    //singleton
    //se llama para instanciar al objeto, o en caso de ya estar construido, devuelve el puntero
    static AssetManager* Instance(SDL2pp::Renderer & renderer);

    //libera el objeto
    static void Release();

    //obtiene el asset con codigo
    Asset* GetAsset(int code);

    //reproduce el sonido con codigo y mixer de parametro
    void play(int code, SDL2pp::Mixer & mixer);  
};

#endif
