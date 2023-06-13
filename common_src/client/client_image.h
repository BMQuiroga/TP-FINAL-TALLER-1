#ifndef CLIENT_IMAGE_H_
#define CLIENT_IMAGE_H_

#include <iostream>
#include <list>

struct Image {
    //clase manejada por ClientRenderer para dibujar un fotograma
    uint8_t id;
    uint8_t action;
    uint8_t flip;
    uint8_t health;
    uint16_t x;
    uint16_t y;
    uint8_t frame;
    uint8_t rounds;
    std::string name;

    //EN DESUSO construye la imagen directamente de la tira de bits
    static std::list<Image>* Create(uint8_t* array);

    //traduce un PSR a image
    explicit Image(const PlayerStateReference &state);

    //traduce un ZSR a image
    explicit Image(const ZombieStateReference &state);

    //constructor
    Image(uint8_t id, uint8_t action, uint8_t flip, uint8_t hp, uint16_t x, uint16_t y);

    //dada una nueva imagen, la compara con la actual para continuar el movimiento de los modelos
    static std::list<Image>* Replace(std::list<Image>* original, std::list<Image>* neww);
};

#endif
