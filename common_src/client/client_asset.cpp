#include <iostream>
#include "client_asset.h"
#include <string>
//#include <arpa/inet.h>

Asset::Asset(std::string path, uint8_t offset, uint8_t height, uint8_t length, uint8_t frames, SDL2pp::Renderer& r, uint32_t colorkey)
    : offset(offset), height(height), length(length), frames(frames) {
    this->texture = new SDL2pp::Texture(r, SDL2pp::Surface(path).SetColorKey(true, colorkey));
}

uint8_t Asset::get_offset() {
    return this->offset;
}

uint8_t Asset::get_height() {
    return this->height;
}

uint8_t Asset::get_length() {
    return this->length;
}

uint8_t Asset::get_frames() {
    return this->frames;
}

SDL2pp::Texture* Asset::get_texture() {
    return this->texture;
}

Asset::~Asset() {
    delete this->texture;
}

