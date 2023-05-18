#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <sstream>
#include <functional>
#include <list>
#include <algorithm>
#include <tuple>
#include "game.h"

Game::Game(int id, const std::string& name) :
    id(id),
    name(name) {
}

void Game::add_player(Queue<MatchState>& q) {
    players.push_back(q);
}

void Game::send_message(const MatchState& state) {
    std::for_each(players.begin(), players.end(),
        [state](Queue<MatchState> & queue)
        {
            queue.push(state);
    });
}

int Game::get_id() const {
    return this->id;
}

bool Game::operator==(const Game &Game) const {
    return id == Game.get_id();
}

bool Game::operator==(const int &code) const {
    return id == code;
}
