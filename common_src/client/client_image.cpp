#include "../player_state.h"
#include "client_image.h"
#include <iostream>

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
            //std::cout << "entity " << id << " at [" << x << " ," << y << "]" << std::endl;
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
    delete[] array;
    return list;
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

Image::Image(const PlayerStateReference &state) {
    std::cout << state.id << "-" << state.state << "-" << state.x << std::endl;
    this->id = state.id;
    this->action = state.state;
    this->flip = state.direction;
    this->health = state.hit_points;
    this->x = state.x;
    this->y = state.y;
    this->frame = 0;
}

std::list<Image>* Image::Replace(std::list<Image>* original, std::list<Image>* neww) {
    if (original != nullptr) {
        std::list<Image>::iterator it1 = original->begin();
        std::list<Image>::iterator it2 = neww->begin();

        while (it1 != original->end() && it2 != neww->end()) {
            if ((*it1).id == (*it2).id) {
                if ((*it1).action == (*it2).action) {
                    (*it2).frame++;
                }
            }
            ++it1;
            ++it2;
        }
        delete original;
        return neww;
    }
    return neww;
}
