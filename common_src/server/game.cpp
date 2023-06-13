#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <sstream>
#include <functional>
#include <list>
#include <algorithm>
#include <utility>
#include <tuple>
#include "game.h"

Game::Game(int id, const std::string& name) :
    id(id),
    name(name),
    events(10000),
    loop(events) {
}

GameReference Game::make_ref() {
    GameReference ref(id, name, players.size());
    return ref;
}

void Game::push_event(
    ProtocolRequest &req,
    std::string &player_name,
    Queue<ProtocolResponse> &player_messages
) {
    GameEvent ev(req, player_name, std::ref(player_messages));
    events.push(std::ref(ev));
}

Game::Game(Game &&other) : 
    events(std::move(other.events)), 
    loop(events), 
    players(other.players), 
    name(other.name) {
    id = other.id;
}

void Game::start() {
    loop.start();
}

void Game::add_player(Queue<ProtocolResponse>& q) {
    players.push_back(q);
}

void Game::notify_all(const ProtocolResponse& state) {
    std::for_each(players.begin(), players.end(),
        [state](Queue<ProtocolResponse> & queue)
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

