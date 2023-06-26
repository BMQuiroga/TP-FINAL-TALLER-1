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

Game::Game(int id, const GameReference &game_ref) :
    id(id),
    name(game_ref.name),
    events(10000),
    number_players(game_ref.players),
    game_mode(game_ref.game_mode),
    loop(events, game_ref.players, game_ref.game_mode) {
}

Game::Game(int id, const std::string& name) :
    id(id),
    name(name),
    events(10000),
    loop(events) {
}

Game::~Game() {
    loop.close();
    loop.join();
}

GameReference Game::make_ref() {
    GameReference ref(id, name, number_players, players_connected);
    return ref;
}

void Game::push_event(
    ProtocolRequest &req,
    std::string &player_name,
    int weapon_code,
    Queue<ProtocolResponse> &player_messages
) {
    if (req.cmd == JOIN) {
        players_connected++;
    }
    GameEvent ev(req, player_name, weapon_code, player_messages);
    events.push(std::ref(ev));
}

Game::Game(Game &&other) : 
    events(std::move(other.events)), 
    loop(events, other.number_players, other.game_mode), 
    players(other.players),
    number_players(other.number_players),
    players_connected(other.players_connected),
    name(other.name) {
    id = other.id;
}

Game& Game::operator=(Game &&other) {
    if (this == &other) {
        return *this;
    }
    events = std::move(other.events);
    players = std::move(other.players);
    number_players = other.number_players;
    players_connected = other.players_connected;
    name = other.name;
    id = other.id;
    return *this;
}

void Game::start() {
    loop.start();
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

GameState Game::get_state() {
    return loop.get_state();
}

bool Game::operator==(const Game &Game) const {
    return id == Game.get_id();
}

bool Game::operator==(const int &code) const {
    return id == code;
}

