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
#include "../game_config.h"

class ClientRenderer {
    //logica de renderizacion, ejecutada sobre el hilo main
    private:
    Queue<Intention*> &events;
    Queue<ProtocolResponse> &updates;
    std::string player_name;
    std::list<Image>* actual_frame;
    AssetManager * assets;
    bool running;
    bool revive_screen;
    bool defeat;
    int offset;

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
    void renderHealth(uint16_t x, uint16_t y, uint8_t hp);

    //maneja eventos para enviarlos al servidor
    bool handleEvents();

    //en el caso que la id sea de sonido, reproduce el sonido y le incrementa los frames para que no se vuelva a reproducir
    void play(Image & im);

    //renderiza la vida y balas propias
    void renderOwn(Image & im);

    //renderiza la pantalla de muerte y mata al juego
    void DeathScreen();

    //renderiza la pantalla de victoria y cierra el juego
    void VictoryScreen();

    //pantalla de revive, entras cuando tu personaje muere, mientras estes en esta pantalla, no podras moverte o disparar
    //reapareceras despues de un tiempo. Si todos los jugadores de una partida estan en muertos, se pierde la partida
    void ReviveScreen();

    //sincroniza el fotograma con respecto de la nueva imagen del server, hace fluidas las animaciones
    uint8_t get_frame(Image & im);

    //calcula un offset para que la camara siga al jugador
    void calculate_offset();

    //renderiza el piso, distinto del fondo
    void render_floor();

    //renderiza las balas, kills y tiempo
    void render_score(uint32_t b, uint32_t k, uint32_t t);

    public:
    explicit ClientRenderer(Queue<Intention*> &events, Queue<ProtocolResponse> &updates, const std::string &player_name);
    //gameloop: renderiza el background y el actual_frame en loop mientras lo actualiza
    void GameLoop();
    ~ClientRenderer();
};

#endif
