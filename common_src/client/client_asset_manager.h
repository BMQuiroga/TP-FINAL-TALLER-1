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
    SDL2pp::Font default_font;
    SDL2pp::Color default_color;
    SDL2pp::Color white_color;
    SDL2pp::Color red_color;

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

    SDL2pp::Color* get_default_color();
    SDL2pp::Color* get_white_color();
    SDL2pp::Color* get_red_color();

    void play_music(SDL2pp::Mixer & mixer);

    SDL2pp::Font* get_default_font();
};

#endif
