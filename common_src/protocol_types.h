#ifndef _PROTOCOL_TYPES_H
#define _PROTOCOL_TYPES_H

#include <string>
#include <vector>

struct PlayerStateReference {
    uint8_t id;
    std::string name;
    uint8_t hit_points;
    uint8_t rounds;
    uint8_t direction;
    uint16_t x;
    uint16_t y;
    int8_t state;
};

struct GameReference {
    int id;
    std::string name;
    uint8_t players;
    GameReference() {}
    GameReference(int id, std::string &name, uint8_t players) :
    id(id), name(name), players(players) {}
};

enum ResponseType {
    GAME_STATE, LOBBY_STATE
};

// Solicitud del cliente
struct ProtocolRequest {
    int cmd; // command (SHOOT, MOVE, etc)
    std::vector<int8_t> content; // request content
};

// Respuesta que se devuelve al cliente despues de cada solicitud
struct ProtocolResponse {
    ProtocolRequest request;
    uint16_t size;               // the total response size, in bytes
    uint8_t content_type; // response content type, useful for deserialization
    std::vector<int8_t> content; // response content
};

struct GameStateResponse {
    int game_state;
    std::vector<PlayerStateReference> players;
    GameStateResponse() = default;
};

struct CreateRequest {
    // int cmd;    // command
    std::string name; // name of the match to create
    int max_number; // number of players that can join the match
    // explicit CreateRequest(std::string &name) : cmd(CREATE), name(name) {}
};

struct InputNameRequest {
    std::string name; // name of the player
};

struct JoinRequest {
    // int cmd;
    int game_code;
    // explicit JoinRequest(int game_code) : cmd(JOIN), game_code(game_code) {}
};

struct LobbyStateResponse {
    std::vector<GameReference> games;
    LobbyStateResponse() = default;
};

#endif
