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

Game& GameHandler::get_game(int code) {
    return matches.get_game(code);
}

size_t GameHandler::get_game_count() {
    return matches.get_game_count();
}

ProtectedMatchesList& GameHandler::get_games() {
    return matches;
}

Game& GameHandler::create_new_game(const std::string& name,
Queue<ProtocolResponse>& q) {
    int game_code(number_matches.get_counter());
    Game new_game(game_code, name);
    // new_game.add_player(q);
    matches.add(new_game);
    number_matches.inc();
    return matches.get_game(new_game.get_id());
}

// void GameHandler::broadcast_message(
//     const std::string &message,
//     const int &game_code) {
//     MatchState state_message("broadcast", game_code, true, message);
//     matches.send_message_to_game(state_message, game_code);
// }

int GameHandler::join_game(int code,
std::string &player_name, Queue<ProtocolResponse> *q) {
    bool game_exists = matches.has(code);
    if (game_exists) {
        Game &game = get_game(code);
        ProtocolRequest req;
        req.cmd = JOIN;
        game.push_event(std::ref(req), player_name, q);
        return JOIN_SUCCESS;
    } else {
        return JOIN_FAILURE;
    }
}

// MatchState GameHandler::start_game_chat(
//     const std::string &command, const std::string &parameters,
//     Queue<MatchState>& q)
// {
//     if (command == "create") {
//         MatchState state(create_new_game(parameters, q));
//         q.push(state);
//         return state;
//     } else if (command == "join") {
//         MatchState state(join_game(std::stoi(parameters), q));
//         q.push(state);
//         return state;
//     }
//     MatchState state("invalid", -1, false);
//     return state;
// }
