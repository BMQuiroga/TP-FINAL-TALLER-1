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

  public:
    // serialization
    std::vector<int8_t> serialize(const GameStateResponse &resp);
    // deserialization
    GameStateResponse deserialize(const std::vector<int8_t> &content);

};

#endif