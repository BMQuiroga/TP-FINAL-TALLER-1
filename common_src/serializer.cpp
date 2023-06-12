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
    const char *ptr = (const char *)(src + offset);
    s = std::string(ptr, ssize);
    return offset+ssize;
}



std::vector<int8_t> Serializer::serialize(const GameStateResponse &resp) {
    std::vector<int8_t> buf;
    push_number(buf, resp.game_state);
    push_number(buf, resp.players.size());
    push_number(buf, resp.zombies.size());
    for (auto player : resp.players) {
        serialize_player(player, buf);
    }
    for (auto zombie : resp.zombies) {
        serialize_zombie(zombie, buf);
    }
    return buf;
}

void Serializer::serialize_player(const PlayerStateReference &player, std::vector<int8_t> &buf) {
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
    push_number(buf, player.rounds);
}

void Serializer::serialize_zombie(const ZombieStateReference &zombie, std::vector<int8_t> &buf) {
    std::cout << "Zombie: " << std::endl << 
    "- id: " << zombie.id << std::endl <<//roto el id
    "- state: " << std::to_string(zombie.state) << std::endl <<
    "- health: " << std::to_string(zombie.health) << std::endl <<
    "- damage: " << std::to_string(zombie.damage) << std::endl <<
    "- x: " << std::to_string(zombie.x) << std::endl <<
    "- y: " << std::to_string(zombie.y) << std::endl <<
    "- direction: " << std::to_string(zombie.direction) << std::endl <<
    "- zombie type: " << std::to_string(zombie.zombie_type) << std::endl <<
    "- attack_type: " << std::to_string(zombie.attack_type) << std::endl <<
    "- movement_type: " << std::to_string(zombie.movement_type) << std::endl;

    push_number(buf, zombie.id);
    push_number(buf, zombie.x);
    push_number(buf, zombie.y);
    push_number(buf, zombie.direction);
    push_number(buf, zombie.health);
    push_number(buf, zombie.state);
    push_number(buf, zombie.damage);
    push_number(buf, zombie.zombie_type);
    push_number(buf, zombie.attack_type);
    push_number(buf, zombie.movement_type);
}

int Serializer::deserialize_player(PlayerStateReference &player_ref, const int8_t *data, int offset) {
    offset += copy_number(data+offset, &player_ref.id);
    offset += copy_number(data+offset, &player_ref.x);
    offset += copy_number(data+offset, &player_ref.y);
    offset += copy_number(data+offset, &player_ref.direction);
    offset += copy_number(data+offset, &player_ref.state);
    offset += copy_number(data+offset, &player_ref.hit_points);
    offset += copy_number(data+offset, &player_ref.rounds);
    return offset;
}

int Serializer::deserialize_zombie(ZombieStateReference &zombie_ref, const int8_t *data, int offset) {
    offset += copy_number(data+offset, &zombie_ref.id);
    offset += copy_number(data+offset, &zombie_ref.x);
    offset += copy_number(data+offset, &zombie_ref.y);
    offset += copy_number(data+offset, &zombie_ref.direction);
    offset += copy_number(data+offset, &zombie_ref.health);
    offset += copy_number(data+offset, &zombie_ref.state);
    offset += copy_number(data+offset, &zombie_ref.damage);
    offset += copy_number(data+offset, &zombie_ref.zombie_type);
    offset += copy_number(data+offset, &zombie_ref.attack_type);
    offset += copy_number(data+offset, &zombie_ref.movement_type);
    return offset; 
}

GameStateResponse Serializer::deserialize_game_state(const std::vector<int8_t> &content) {
    GameStateResponse resp;
    const int8_t *data = content.data();
    size_t number_players;
    size_t number_zombies; 
    data += copy_number(data, &resp.game_state);
    int offset = 0, size = content.size() - sizeof(resp.game_state);
    offset += copy_number(data+offset, &number_players);
    offset += copy_number(data+offset, &number_zombies);
    // while (size > offset) {
    for (int i = 0; i < number_players; i++) {
        PlayerStateReference player_ref;
        offset = deserialize_player(player_ref, data, offset);
        resp.players.push_back(player_ref);
    }

    for (int i = 0; i < number_zombies; i++) {
        ZombieStateReference zombie_ref;
        offset = deserialize_zombie(zombie_ref, data, offset);
        resp.zombies.push_back(zombie_ref);
    }

    return resp;
}

std::vector<int8_t> Serializer::serialize(const GameReference &ref) {
    std::vector<int8_t> buf;
    push_number(buf, ref.id);
    push_string(buf, ref.name);
    push_number(buf, ref.players);
    return buf;
}

GameReference Serializer::deserialize_game_reference(const std::vector<int8_t> &content) {
    int offset = 0;
    const int8_t *data = content.data();
    GameReference ref;
    offset += copy_number(data+offset, &ref.id);
    offset += copy_string(data+offset, ref.name);
    offset += copy_number(data+offset, &ref.players);
    return ref;
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
    JoinRequest resp;
    const int8_t *data = content.data();
    int offset = 0, size = content.size();
    std::string test("");
    offset += copy_number(data+offset, &resp.game_code);
    return resp;
}
