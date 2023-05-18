#include <iostream>
#include <exception>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <tuple>
#include <stdint.h>
#include <string>
#include <sstream>
#include <cstdint>
#include <cstdlib>
#include "queue.h"
#include "client_receiver.h"

ClientReceiver::ClientReceiver(Socket& socket,
    Queue<MatchState>& q,
    ServerProtocol& protocol,
    GameHandler &game_handler) :
    skt(socket),
    q(q),
    protocol(protocol),
    game_code(-1),
    game_handler(game_handler)
{
}

void ClientReceiver::run()
{
    char command[35] = {0};
    is_alive = keep_talking = true;
    while (keep_talking) {
        std::tuple command_info = protocol.receive(
            skt, command, keep_talking);
        std::string command_name = std::get<0>(command_info);
        std::string command_args = std::get<1>(command_info);
        if (!keep_talking) {
            kill();
            break;
        }
        send_command(command_name, command_args);
    }
}

bool ClientReceiver::is_dead()
{
    return not is_alive;
}

void ClientReceiver::kill()
{
    keep_talking = false;
    q.push(MatchState{"", -1, false});
}

void ClientReceiver::send_command(
    const std::string &command_name, const std::string &command_args) {
    if (command_name != "broadcast") { // Hago el handshake
        //Esto está raro. Teniendo en cuenta que un cliente pertenece a la misma
        //partida el resto de su vida, ese start_game_chat podría hacerse al
        //principio de la partida, y una vez que el cliente se unió, se proceden
        //a hacer los broadcasts normales.
        //Aunque viendo un poco más el código pareciera que lo planteaste
        //para que los usuarios se puedan cambiar de partida fácilmente.
        //Me parece q no era algo necesario pero está bien
        MatchState state = game_handler.start_game_chat(
            command_name, command_args,q);
        if (state.succeeded) {
            game_code = state.match_code;
        }
    } else {
        game_handler.broadcast_message(
            command_args, game_code);            
    }
}
