#include <iostream>
#include "client_renderer.h"
#include <string>
#include "intention.h"
#include "button.h"
#include <cmath>
#include <vector>
#include "../serialization.h"
//#include <arpa/inet.h>
#include "../game_config.h"

void ClientRenderer::render_floor() {
    Asset * asset = assets->GetAsset(-8);
    renderer.Copy(
        (*asset->get_texture()),
        SDL2pp::Rect(0, 300, 5760, 780),
        SDL2pp::Point(offset,0)
    );
}

void ClientRenderer::render_score(uint32_t b, uint32_t k, uint32_t t) {
    std::cout << "RENDERSCORE: " << std::to_string(b) << " " << std::to_string(k) << " " << std::to_string(t) << std::endl;
    /*std::string text_s = "Score: " + std::to_string((k*100) - t);
    std::string text_k = "Kills: " + std::to_string(k);
    std::string text_b = "Times Shot: " + std::to_string(b);
    std::string text_t = "Time: " + std::to_string(t);

    Button button_s(text_s,SDL2pp::Rect(50,830,20*text_s.size(),40));
    Button button_k(text_k,SDL2pp::Rect(50,880,20*text_k.size(),40));
    Button button_b(text_b,SDL2pp::Rect(50,930,20*text_b.size(),40));
    Button button_t(text_t,SDL2pp::Rect(50,980,20*text_t.size(),40));
    
    SDL2pp::Color* white = assets->get_white_color();
    //SDL2pp::Font font("../resources/Fonts/ARIAL.TTF", 30);

    button_s.Render(this->renderer,white);
    button_k.Render(this->renderer,white);
    button_b.Render(this->renderer,white);
    button_t.Render(this->renderer,white);*/
}

ClientRenderer::ClientRenderer(Queue<Intention*> &events, Queue<ProtocolResponse> &updates, const std::string &player_name) : 
    events(events),
    updates(updates),
    player_name(player_name),
    running(true),
    defeat(false),
    revive_screen(false),
    sdl(SDL_INIT_VIDEO),
    window("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_RESIZABLE),
    actual_frame(nullptr),
    renderer(window, -1, SDL_RENDERER_ACCELERATED),
    mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) {
    this->assets = AssetManager::Instance(this->renderer);
    Mix_VolumeMusic(MIN_MAX_VOLUME / 100 * GAME_VOLUME);
    std::cout << "player name is " << this->player_name << std::endl;
    //mixer.OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}

void ClientRenderer::GameLoop() {
    // std::list<Image>* new_update = nullptr;
    int wait_time = 1000/GAME_FRAME_RATE;
    std::list<Image>* frames_list = nullptr;
    //this->assets->play_music(this->mixer); //anda horrible
    ProtocolResponse new_update;
    Serializer serializer;
    while (running) {
        unsigned int frame_ticks = SDL_GetTicks();	
        running = this->handleEvents();
        renderer.Clear();
        if (updates.try_pop(new_update)) {
            frames_list = new std::list<Image>;
            std::vector<PlayerStateReference>::iterator it;
            std::vector<ZombieStateReference>::iterator it_zombies;
            if (new_update.content_type == GAME_STATE) {
                GameStateResponse update = serializer.deserialize_game_state(new_update.content);
                for (PlayerStateReference &player : update.players) {
                    std::cout << "Player: " << std::endl << 
                        "- id: " << std::to_string(player.id) << std::endl <<
                        "- name: " << player.name << std::endl <<
                        "- state: " << std::to_string(player.state) << std::endl <<
                        "- hit points: " << std::to_string(player.hit_points) << std::endl <<
                        "- x: " << std::to_string(player.x) << std::endl <<
                        "- y: " << std::to_string(player.y) << std::endl;
                }
                for (ZombieStateReference &zombie : update.zombies) {
                    std::cout << "Zombie: " << std::endl << 
                        "- id: " << zombie.id << std::endl <<
                        "- type: " << zombie.zombie_type << std::endl <<
                        "- state: " << std::to_string(zombie.state) << std::endl <<
                        "- health: " << std::to_string(zombie.health) << std::endl <<
                        "- damage: " << std::to_string(zombie.damage) << std::endl <<
                        "- x: " << std::to_string(zombie.x) << std::endl <<
                        "- y: " << std::to_string(zombie.y) << std::endl;
                }
                for (it = update.players.begin(); it != update.players.end(); ++it) {
                    auto new_Model = Image((*it));
                    new_Model.frame = get_frame(new_Model);
                    frames_list->push_back(new_Model);
                }
                for (it_zombies = update.zombies.begin(); it_zombies != update.zombies.end(); ++it_zombies) {
                    auto new_Model = Image((*it_zombies));
                    new_Model.frame = get_frame(new_Model);
                    frames_list->push_back(new_Model);
                }
                render_score(update.shots, update.kills, update.time);
                //render_score(1,1,1);
            } else if (new_update.content_type == LOBBY_STATE) {
                // LobbyStateResponse update = serializer.deserialize(new_update.content);
            }
            //std::cout << "created image" << std::endl;
            //this->actual_frame = Image::Replace(this->actual_frame,frames_list);
            if (actual_frame)
                delete actual_frame;
            this->actual_frame = frames_list;
            //std::cout << "replaced image" << std::endl;
            //std::cout << actual_frame->size() << std::endl;
        }
        renderBackground();
        render_all();
        if (revive_screen)
            ReviveScreen();
        renderer.Present();
        unsigned int end_ticks = SDL_GetTicks();
        unsigned int ticks_delta = frame_ticks - end_ticks;
        if (wait_time > ticks_delta)
            SDL_Delay(wait_time - ticks_delta);
    }
    if (defeat)
        DeathScreen();
}

uint8_t ClientRenderer::get_frame(Image & im) {
    if (!actual_frame)
        return 0;
    std::list<Image>::iterator it1 = actual_frame->begin();
    while (it1 != actual_frame->end()) {
        if ((*it1).id == im.id) {
            if ((*it1).action == im.action) {
                return (*it1).frame + 1;
            }
        }
        ++it1;
    }
    return 0;
}

void ClientRenderer::render_all() {
    if (this->actual_frame != nullptr) {
        calculate_offset();
        render_floor();;
        for (auto const& it : *actual_frame) {
            //std::cout << "id:" << it.id << std::endl;
            if (it.id > 0 && it.id < 151) {
                render(const_cast<Image&>(it));
            } else if (it.id > 150 && it.id < 251){
                if (it.frame == 0) {
                    play(const_cast<Image&>(it));
                }
            } else if (it.id == 251){
                VictoryScreen();
            } else if (it.id == 252){
                defeat = true;
                running = false;
            }
        }
    }
}

void ClientRenderer::play(Image & im) {
    assets->play(im.id,this->mixer);
}

void ClientRenderer::calculate_offset() {
    for (auto const& it : *actual_frame) {
        if (it.name == player_name) {
            //offset = (RESOLUTION_X/2) - it.x;
            //int a = (RESOLUTION_X / 2) - it.x;;
            //int b = DEFAULT_MAX_X - RESOLUTION_X;
            //offset = std::max(std::min(a,0), std::min(b,0));
            int fix = RESOLUTION_X/2;
            int max = DEFAULT_MAX_X + 128;//128 es el tamaño del modelo del jugador
            int x = it.x;
            if (x < fix)
                offset = 0;
            else if (x > (max - fix))
                offset = RESOLUTION_X - max;
            else
                offset = fix - x;

            return;
        }      
    }
}

void ClientRenderer::render(Image & im) {
    uint16_t x = im.x + offset;
    uint16_t y = im.y;
    Asset * asset = assets->GetAsset(im.id + im.action*1000);

    im.frame++;
    while (im.frame >= asset->get_frames()) {
        if (im.action != 2)
            im.frame -=  asset->get_frames();
        else
            im.frame = asset->get_frames() -1;
            //el modelo al morir, se queda tirado en el piso, sin repetir la animacion de muerte
    }

    renderer.Copy(
        (*asset->get_texture()),
        SDL2pp::Rect(asset->get_length() * im.frame, 0, asset->get_length(), asset->get_height()),
        SDL2pp::Rect(x, y + Y_OFFSET, asset->get_length() - 1, asset->get_height() - 1),
        0,
        SDL2pp::NullOpt,
        im.flip > 0 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);

    if (im.name == player_name) {
        renderOwn(im);
    } else if (im.health != 0) {
        renderHealth(x,y,im.health);
    }
}

void ClientRenderer::renderOwn(Image & im) {
    if (im.health == 0)
        revive_screen = true;
    else
        revive_screen = false;

    int hearts = im.health / 10;
    Asset * asset = assets->GetAsset(-3);

    for (int i = 0; i < hearts; i++) {
        renderer.Copy(
            (*asset->get_texture()),
            SDL2pp::NullOpt,
            SDL2pp::Rect(350 + 55*i, 830, 50, 50)
        );
    }

    Asset * asset2 = assets->GetAsset(-4);

    for (int i = 0; i < im.rounds; i++) {
        renderer.Copy(
            (*asset2->get_texture()),
            SDL2pp::NullOpt,
            SDL2pp::Rect(350 + 20*i, 930, 50, 50)
        );
    }
}

void ClientRenderer::renderHealth(uint16_t x, uint16_t y, uint8_t hp) {
    Asset * full = assets->GetAsset(-1);
    /*Asset * empty = assets->GetAsset(-2);*/
    float hp_percentage = 50*hp/100;

    /*renderer.Copy(
        (*empty->get_texture()),
        SDL2pp::Rect(0,0,50,HP_BAR_HEIGHT),
        SDL2pp::Rect(x , y + (HP_BAR_HEIGHT*2) + Y_OFFSET, HP_BAR_LENGTH - 1, HP_BAR_HEIGHT - 1));*/
    renderer.Copy(
        (*full->get_texture()),
        SDL2pp::Rect(0,0,HP_BAR_LENGTH,HP_BAR_HEIGHT),
        SDL2pp::Rect(x , y + (HP_BAR_HEIGHT*2) + Y_OFFSET, std::round(hp_percentage) - 1, HP_BAR_HEIGHT - 1));
}

void ClientRenderer::renderBackground() {
    Asset * asset = assets->GetAsset(0);
    renderer.Copy(
        (*asset->get_texture()),
        SDL2pp::Rect(0, 300, 1920, 780),
        SDL2pp::Point(0,0)
    );
    renderer.SetDrawColor(*assets->get_default_color());
    renderer.FillRect(SDL2pp::Rect(0, 780, 1920, 300));
}

ClientRenderer::~ClientRenderer() {
    AssetManager::Release();
}

bool ClientRenderer::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        Intention *command(Intention::make_intention(event));
        if (command != nullptr) {
            events.push(command);
            if (command->get_intention() == END) {
                return false;
            }
        }
    }
    return true;
}


void ClientRenderer::DeathScreen() {
    assets->play(152,this->mixer);
    Asset * asset = assets->GetAsset(-5);
    for (int i = 0; i < GAME_FRAME_RATE*5 ; i++) {
        renderer.Clear();
        renderer.Copy(
            (*asset->get_texture()),
            SDL2pp::Rect(0, 0, 1920, 1080),
            SDL2pp::Point(0,0)
        );
        renderer.Present();
    }
}


void ClientRenderer::VictoryScreen() {
    running = false;
    assets->play(153,this->mixer);
    Asset * asset = assets->GetAsset(-6);
    for (int i = 0; i < GAME_FRAME_RATE*5 ; i++) {
        renderer.Clear();
        renderer.Copy(
            (*asset->get_texture()),
            SDL2pp::Rect(0, 0, 1920, 1080),
            SDL2pp::Point(0,0)
        );
        renderer.Present();
    }
}


void ClientRenderer::ReviveScreen() {
    Asset * asset = assets->GetAsset(-7);
    renderer.Copy(
        (*asset->get_texture()),
        SDL2pp::Rect(0, 0, 1920, 1080),
        SDL2pp::Point(0,0)
    );
}
