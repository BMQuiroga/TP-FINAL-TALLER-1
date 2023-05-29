#ifndef CLIENT_ASSET_H_
#define CLIENT_ASSET_H_

#include "../socket.h"
#include "../protocol.h"
#include <string>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/Renderer.hh>


class Asset {
    private:
    SDL2pp::Texture * texture;
    uint8_t offset;
    uint8_t height;
    uint8_t length;
    uint8_t frames;

    public:
    Asset(std::string path, uint8_t offset, uint8_t height, uint8_t length, uint8_t frames, SDL2pp::Renderer& r, uint32_t colorkey);

    uint8_t get_offset();

    uint8_t get_height();

    uint8_t get_length();

    uint8_t get_frames();

    SDL2pp::Texture* get_texture();

    ~Asset();
};

#endif
