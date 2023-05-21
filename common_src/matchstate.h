#ifndef _MATCHSTATE_H
#define _MATCHSTATE_H

#include <string>
#include <vector>

enum direction {
    LEFT,
    RIGHT
};

struct MatchState {
    uint8_t direction;
    bool is_shooting;
    uint8_t rounds;
    uint16_t hit_points;
    std::vector<int> position;

    MatchState(
    uint8_t direction,
    bool is_shooting,
    uint8_t rounds,
    uint16_t hit_points,
    int x, int y) : 
        direction(direction), is_shooting(is_shooting), rounds(rounds), hit_points(hit_points), position{x,y} {}
};
#endif
