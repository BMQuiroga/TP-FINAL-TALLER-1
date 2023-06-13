#ifndef CLIENT_RENDERER_H_
#define CLIENT_RENDERER_H_

#include "../socket.h"
#include "../protocol.h"
#include <string>
#include <memory>
#include <list>
#include "intention.h"
#include "client_asset_manager.h"
#include "client_image.h"

class ClientRenderer {
    //logica de renderizacion, ejecutada sobre el hilo main
    private:
    Queue<Intention*> &events;
    Queue<ProtocolResponse> &updates;
    std::string player_name;
    std::list<Image>* actual_frame;
    AssetManager * assets;

    SDL2pp::SDL sdl;
    SDL2pp::Window window;
    SDL2pp::Renderer renderer;
    SDL2pp::Mixer mixer;

    //renderiza la lista de imagenes actual_frame, llama n veces a render o play
    void render_all();

    //renderiza una imagen de la lista
    void render(Image & im);

    //renderiza el fondo
    void renderBackground();

    //renderiza una barra de vida dada la longitud, coordenadas y health points
    void renderHealth(uint16_t length, uint16_t x, uint16_t y, uint8_t hp);

    //maneja eventos para enviarlos al servidor
    bool handleEvents();

    //en el caso que la id sea de sonido, reproduce el sonido y le incrementa los frames para que no se vuelva a reproducir
    void play(Image & im);

    public:
    explicit ClientRenderer(Queue<Intention*> &events, Queue<ProtocolResponse> &updates, const std::string &player_name);
    //gameloop: renderiza el background y el actual_frame en loop mientras lo actualiza
    void GameLoop();
    ~ClientRenderer();
};

#endif
