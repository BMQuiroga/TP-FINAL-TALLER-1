#ifndef _SERIALIZATION_H
#define _SERIALIZATION_H

#include "protocol_types.h"
#include <vector>
#include <string>

class Serializer {
  //clase que convierte un protocol_type a tira de bytes o viceversa
  public:
    // serialization
    template <typename T> void push_number(std::vector<int8_t> &buf, T num);
    void push_string(std::vector<int8_t> &buf, const std::string &s);
    // deserialization
    template <typename T> int copy_number(const int8_t *src, T* dest);
    int copy_string(const int8_t *src, std::string &s);
    // serialization
    std::vector<int8_t> serialize(const GameStateResponse &resp);
    std::vector<int8_t> serialize(const GameReference &ref);
    std::vector<int8_t> serialize(const LobbyStateResponse &resp);
    std::vector<int8_t> serialize(const LobbyGameStateResponse &resp);
    // deserialization
    void serialize_player(const PlayerStateReference &player, std::vector<int8_t> &buf);
    void serialize_zombie(const ZombieStateReference &zombie, std::vector<int8_t> &buf);
    int deserialize_player(PlayerStateReference &player_ref, const int8_t *data, int offset);
    int deserialize_zombie(ZombieStateReference &zombie_ref, const int8_t *data, int offset);
    GameStateResponse deserialize_game_state(const std::vector<int8_t> &content);
    LobbyStateResponse deserialize_lobby_state(const std::vector<int8_t> &content);
    LobbyGameStateResponse deserialize_join_response(const std::vector<int8_t> &content);
    CreateRequest deserialize_create_state(const std::vector<int8_t> &content);
    NewPlayerRequest deserialize_input_name(const std::vector<int8_t> &content);
    JoinRequest deserialize_join_state(const std::vector<int8_t> &content);
    GameReference deserialize_game_reference(const std::vector<int8_t> &content);
};

#endif
