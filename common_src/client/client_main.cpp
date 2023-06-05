#include "client_protocol.h"
#include "../socket.h"
#include "../queue.h"
#include "client_receiver.h"
#include "mainmenu.h"
#include "client_sender.h"
#include "client_renderer.h"
#include <iostream>
#include <exception>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <functional>


int main(int argc, char *argv[]) { try {
    int ret = 1;
    const char *hostname = NULL;
    const char *servname = NULL;

    if (argc == 3) {
        hostname = argv[1];
        servname = argv[2];
    } else {
        std::cerr << "Bad program call. Expected "
                  << argv[0]
                  << " <ip/hostname server> <puerto/servicename>.\n";
        return ret;
    }

    MainMenu main;
    if (main.Start() == -1) {
        return ret;
    }
        
    Queue<Intention*> events_q(1000);
    Queue<ProtocolResponse> updates_q(1000);
    Socket skt(hostname, servname);
    ClientSender client_sender(std::ref(skt), std::ref(events_q));
    ClientReceiver client_receiver(std::ref(skt), std::ref(updates_q));
    client_sender.start();
    client_receiver.start();
    ClientRenderer client_renderer(std::ref(events_q),std::ref(updates_q));
    client_renderer.GameLoop(); //<--- magia
    skt.shutdown(0);
    skt.close();
    client_sender.join();
    client_receiver.kill();
    client_receiver.join();
    ret = 0;
    return ret;
} catch (const std::exception& err) {
    std::cerr
        << "Something went wrong and an exception was caught: "
        << err.what()
        << "\n";
    return -1;
} catch (...) {
    std::cerr << "Something went wrong and an unknown exception was caught.\n";
    return -1;
} }
