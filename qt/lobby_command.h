#ifndef LOBBY_COMMAND_H
#define LOBBY_COMMAND_H
#define INPUTNAME "inputName"
#define JOINGAME "joinGame"
#define GAMESLIST "gamesList"
#define CREATEGAME "createGame"
#define ENDLOBBY "end"
#include <string>
struct LobbyCommand
{
    std::string name;
    std::string parameter;
    int parameter2;
    int parameter3;
    LobbyCommand(
        const std::string &name, 
        const std::string &parameter, 
        int parameter2 = 0, int parameter3 = 0) : 
        name(name), parameter(parameter), parameter2(parameter2), parameter3(parameter3) {}
};
#endif
