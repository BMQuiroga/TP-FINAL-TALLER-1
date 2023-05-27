#include "client_image.h"

std::list<Image>* Image::Create(uint8_t* array) {
    std::list<Image>* list = new std::list<Image>;
    uint8_t * it = array;
    it++;
    list->push_back(Image((*it++),(*it++),0,0,0,0));
    while (it) {
        if ((*it) < 151) {
            uint8_t id = (*it++);
            uint16_t * it16 = reinterpret_cast<uint16_t*>(it);
            uint16_t x = (*it16++);
            uint16_t y = (*it16++);
            it+=4;
            uint8_t flip = (*it++);
            uint8_t action = (*it++);
            uint8_t health = (*it++);
            list->push_back(Image(id, action, flip, health, x, y));
        }
        if ((*it) > 150) {
            uint8_t id = (*it++);
            uint16_t * it16 = reinterpret_cast<uint16_t*>(it);
            uint16_t x = (*it16++);
            uint16_t y = (*it16++);
            it+=4;
            uint8_t flip = (*it++);
            list->push_back(Image(id, 0, flip, 0, x, y));
        }
    }
}

Image::Image(uint8_t id, uint8_t action, uint8_t flip, uint8_t hp, uint16_t x, uint16_t y) {
    this->id = id;
    this->action = action;
    this->flip = flip;
    this->health = hp;
    this->x = x;
    this->y = y;
    this->frame = 0;
}