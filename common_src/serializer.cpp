#include "serialization.h"
#include <algorithm>
#include <iostream>
#include <cstring>
#include <arpa/inet.h> 

template <typename T> void 
Serializer::push_number(std::vector<int8_t> &buf, T num) {
    if (sizeof(num) == sizeof(uint16_t))
        num = htons(num);
    else if (sizeof(num) >= sizeof(uint32_t))
        num = htonl(num);
    int iter = sizeof(num) / sizeof(uint8_t);
    int8_t *bytes = static_cast<int8_t *>(static_cast<void *>(&num));
    for (int i = 0; i < iter; i++) {
        buf.push_back(bytes[i]);
    }
}

void Serializer::push_string(std::vector<int8_t> &buf, const std::string &s) {
    push_number(buf, (uint16_t)s.size());
    std::copy_if(s.begin(), s.end(), std::back_inserter(buf), [](char c) { return c != '\0'; });
}

template <typename T> int 
Serializer::copy_number(const int8_t *src, T* dest) {
    std::memcpy(dest, src, sizeof(*dest));
    if (sizeof(*dest) == sizeof(uint16_t))
        *dest = ntohs(*dest);
    else if (sizeof(*dest) >= sizeof(uint32_t))
        *dest = ntohl(*dest);

    return sizeof(*dest);
}

int Serializer::copy_string(const int8_t *src, std::string &s) {
    uint16_t offset = 0, ssize;
    offset += copy_number(src, &ssize);
    std::memcpy(&s[0], src + offset, ssize);
    return offset+ssize;
}

std::vector<int8_t> Serializer::serialize(const GameStateResponse &resp) {
    std::vector<int8_t> buf;
    push_number(buf, resp.game_state);
    for (auto player : resp.players) {
        std::cout << "Player: " << std::endl << 
            "- id: " << player.id << std::endl <<//roto el id
            "- name: " << player.name << std::endl <<
            "- state: " << std::to_string(player.state) << std::endl <<
            "- hit points: " << std::to_string(player.hit_points) << std::endl <<
            "- x: " << std::to_string(player.x) << std::endl <<
            "- y: " << std::to_string(player.y) << std::endl <<
            "- direction: " << std::to_string(player.direction) << std::endl;
        push_number(buf, player.id);
        push_number(buf, player.x);
        push_number(buf, player.y);
        push_number(buf, player.direction);
        push_number(buf, player.state);
        push_number(buf, player.hit_points);
    }
    return buf;
}

GameStateResponse Serializer::deserialize_game_state(const std::vector<int8_t> &content) {
    GameStateResponse resp;
    const int8_t *data = content.data();
    data += copy_number(data, &resp.game_state);
    int offset = 0, size = content.size() - sizeof(resp.game_state);
    while (size > offset) {
        PlayerStateReference ref;
        offset += copy_number(data+offset, &ref.id);
        offset += copy_number(data+offset, &ref.x);
        offset += copy_number(data+offset, &ref.y);
        offset += copy_number(data+offset, &ref.direction);
        offset += copy_number(data+offset, &ref.state);
        offset += copy_number(data+offset, &ref.hit_points);
        resp.players.push_back(ref);
    }

    return resp;
}

std::vector<int8_t> Serializer::serialize(const LobbyStateResponse &resp) {
    std::vector<int8_t> buf;
    for (auto game : resp.games) {
        std::cout << "Game: " << std::endl << 
            "- id: " << game.id << std::endl <<//roto el id
            "- name: " << game.name << std::endl <<
            "- players: " << std::to_string(game.players) << std::endl;
        push_number(buf, game.id);
        push_string(buf, game.name);
        push_number(buf, game.players);
    }
    return buf;
}


LobbyStateResponse Serializer::deserialize_lobby_state(const std::vector<int8_t> &content) {
    LobbyStateResponse resp;
    const int8_t *data = content.data();
    int offset = 0, size = content.size();
    std::string test("");
    while (size > offset) {
        GameReference ref(0, test, 0);
        offset += copy_number(data+offset, &ref.id);
        offset += copy_string(data+offset, ref.name);
        offset += copy_number(data+offset, &ref.players);
        resp.games.push_back(ref);
    }
    return resp;
}

CreateRequest Serializer::deserialize_create_state(const std::vector<int8_t> &content)
{
    CreateRequest resp;
    const int8_t *data = content.data();
    int offset = 0, size = content.size();
    std::string test("");
    offset += copy_number(data+offset, &resp.max_number);
    offset += copy_string(data+offset, resp.name);
    return resp;
}

InputNameRequest Serializer::deserialize_input_name(const std::vector<int8_t> &content)
{
    InputNameRequest resp;
    const int8_t *data = content.data();
    int offset = 0, size = content.size();
    std::string test("");
    offset += copy_string(data+offset, resp.name);
    return resp;
}

JoinRequest Serializer::deserialize_join_state(const std::vector<int8_t> &content)
{
    // return JoinRequest();
}
