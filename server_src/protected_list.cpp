#include "protected_list.h"

bool ProtectedMatchesList::has(int code)  {
    m.lock();
    Game searchGame(code, "find game");
    auto it = std::find(list.begin(), list.end(), searchGame);
    bool code_exists = it != list.end() ? true : false;
    m.unlock();
    return code_exists;
}

void ProtectedMatchesList::add(const Game &x) {
        m.lock();
        list.push_back(x);
        m.unlock();
}

Game& ProtectedMatchesList::get_game(int code) {
    m.lock();
    Game searchGame(code, "find game");
    auto it = std::find(list.begin(), list.end(), searchGame);
    m.unlock();
    return *it;
}

void ProtectedMatchesList::send_message_to_game(
    const MatchState &message, int code) {
    m.lock();
    auto it = std::find(list.begin(), list.end(), code);
    (*it).send_message(message);
    m.unlock();
}

void ProtectedMatchesList::change_game_players(
    int code, Queue<MatchState>& q) {
    m.lock();
    auto it = std::find(list.begin(), list.end(), code);
    (*it).add_player(q);
    m.unlock();
}
