#include "gamehandler.h"
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include "../queue.h"
#include "../player_state.h"

GameHandler::GameHandler(int number_matches) :
    number_matches(number_matches) {
}

Game* GameHandler::get_game(int code) {
    Game searchGame(code, "find game");
    auto it = std::find(games.begin(), games.end(), searchGame);
    if (it != games.end()) {
        return &*it;
    }
    return nullptr;
}

size_t GameHandler::get_game_count() {
    return games.size();
}

ProtectedVector<Game>& GameHandler::get_games() {
    return games;
}

std::vector<GameReference> GameHandler::get_refs() {
    std::vector<GameReference> refs = games.call<std::vector<GameReference>>([](std::vector<Game> &_games) {
        std::vector<GameReference> refs;
        for (auto &game : _games) {
            refs.push_back(game.make_ref());
        }

        return refs; 
    });
    return refs;
}

Game* GameHandler::create_new_game(const GameReference& game_ref,
Queue<ProtocolResponse>& q) {
    Game new_game(number_matches++, game_ref);
    auto ref = std::ref(new_game);
    games.call([ref](std::vector<Game> &_games) {
        _games.push_back(std::move(ref.get()));
    });
    return get_game(new_game.get_id());
}

int GameHandler::join_game(int code,
std::string &player_name, int weapon_code, Queue<ProtocolResponse> &q) {
    Game* game = get_game(code);
    if (game) {
        ProtocolRequest req;
        req.cmd = JOIN;
        game->push_event(std::ref(req), player_name, weapon_code, q);
        return JOIN_SUCCESS;
    } else {
        return JOIN_FAILURE;
    }
}
