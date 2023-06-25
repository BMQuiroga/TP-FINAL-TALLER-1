#include <gtest/gtest.h>
#include "../common_src/serialization.h"
#include "../common_src/zombie.h"
#include "../common_src/server/game.h"

TEST(ProtocolTest, TestGameStateSerialization) {
    Serializer serializer;
    PlayerState player("test player", 1, 1);
    CommonZombie zombie("test zombie", 1, 1);
    GameStateResponse resp;
    resp.game_state = STARTED;
    resp.kills = 10;
    resp.players.push_back(player.make_ref());
    resp.shots = 10;
    resp.time = 10;
    resp.zombies.push_back(zombie.make_ref());
    std::vector<int8_t> content = serializer.serialize(resp); // bytes to send to the client
    GameStateResponse received = serializer.deserialize_game_state(content);
    
    ASSERT_EQ(resp.game_state, received.game_state);
    ASSERT_EQ(resp.kills, received.kills);
    ASSERT_EQ(resp.players.size(), received.players.size());
    for (int i = 0; i < resp.players.size(); i++) {
        ASSERT_EQ(resp.players[i].name, received.players[i].name);
        ASSERT_EQ(resp.players[i].hit_points, received.players[i].hit_points);
        ASSERT_EQ(resp.players[i].id, received.players[i].id);
        ASSERT_EQ(resp.players[i].rounds, received.players[i].rounds);
        ASSERT_EQ(resp.players[i].state, received.players[i].state);
        ASSERT_EQ(resp.players[i].weapon_code, received.players[i].weapon_code);
        ASSERT_EQ(resp.players[i].x, received.players[i].x);
        ASSERT_EQ(resp.players[i].y, received.players[i].y);
    }
    ASSERT_EQ(resp.shots, received.shots);
    ASSERT_EQ(resp.time, received.time);
    ASSERT_EQ(resp.zombies.size(), received.zombies.size());
    for (int i = 0; i < resp.zombies.size(); i++) {
        ASSERT_EQ(resp.zombies[i].zombie_type, received.zombies[i].zombie_type);
        ASSERT_EQ(resp.zombies[i].health, received.zombies[i].health);
        ASSERT_EQ(resp.zombies[i].id, received.zombies[i].id);
        ASSERT_EQ(resp.zombies[i].movement_type, received.zombies[i].movement_type);
        ASSERT_EQ(resp.zombies[i].state, received.zombies[i].state);
        ASSERT_EQ(resp.zombies[i].direction, received.zombies[i].direction);
        ASSERT_EQ(resp.zombies[i].x, received.zombies[i].x);
        ASSERT_EQ(resp.zombies[i].y, received.zombies[i].y);
    }
}