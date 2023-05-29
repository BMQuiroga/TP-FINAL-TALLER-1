#ifndef MATCHSTATE_H
#define MATCHSTATE_H
#include <string>

struct MatchState {
    std::string command;
    int match_code;
    bool succeeded;
    std::string message;

    MatchState() = default;
    MatchState(
    const std::string &cmd, 
    int code, bool success, 
    const std::string &msg = "") : 
        command(cmd), match_code(code), succeeded(success), message(msg) {}
};
#endif
