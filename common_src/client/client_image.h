#ifndef CLIENT_IMAGE_H_
#define CLIENT_IMAGE_H_

#include <iostream>
#include <list>

struct Image {
    uint8_t id;
    uint8_t action;
    uint8_t flip;
    uint8_t health;
    uint16_t x;
    uint16_t y;
    uint8_t frame;

    static std::list<Image>* Create(uint8_t* array);
    explicit Image(const PlayerStateReference &state);
    explicit Image(const ZombieStateReference &state);
    Image(uint8_t id, uint8_t action, uint8_t flip, uint8_t hp, uint16_t x, uint16_t y);
    static std::list<Image>* Replace(std::list<Image>* original, std::list<Image>* neww);
};

#endif
