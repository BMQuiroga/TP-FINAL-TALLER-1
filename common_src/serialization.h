#ifndef _SERIALIZATION_H
#define _SERIALIZATION_H

#include "protocol_types.h"
#include <vector>

class Serializer {    
  private:
    // serialization
    template <typename T> void push_number(std::vector<int8_t> &buf, T num);
    void push_string(std::vector<int8_t> &buf, const std::string &s);
    // deserialization
    template <typename T> int copy_number(const int8_t *src, T* dest);
    int copy_string(const int8_t *src, std::string &s);

  public:
    // serialization
    std::vector<int8_t> serialize(const GameStateResponse &resp);
    std::vector<int8_t> serialize(const LobbyStateResponse &resp);
    // deserialization
    GameStateResponse deserialize_game_state(const std::vector<int8_t> &content);
    LobbyStateResponse deserialize_lobby_state(const std::vector<int8_t> &content);
};

#endif