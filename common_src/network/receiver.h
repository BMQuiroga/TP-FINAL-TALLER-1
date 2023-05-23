
#ifndef _RECEIVER_H
#define _RECEIVER_H

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
#include <atomic>
#include <functional>
#include "../socket.h"
#include "../queue.h"
#include "../thread.h"
#include "../protocol.h"

template <typename Send_Type, typename Recv_Type>
class Receiver:public Thread {
private:
    Socket& skt;
    Queue<Send_Type>& q;
    Protocol<Send_Type, Recv_Type>& protocol;
    std::atomic<bool> keep_talking;
    std::atomic<bool> is_alive;
    std::vector<std::function<void(Recv_Type& message)>> callbacks;

public:
    explicit Receiver(Socket& socket,
        Queue<Send_Type>& q,
        Protocol<Send_Type, Recv_Type>& protocol) : 
        skt(socket),
        q(q),
        protocol(protocol) {}

    void register_callback(std::function<void(Recv_Type& message)> callback) {
        callbacks.push_back(callback);
    }

    void run() override {
        is_alive = keep_talking = true;
        while (keep_talking) {
            auto message = protocol.get(skt, keep_talking);
            if (!keep_talking) {
                kill();
                break;
            }
            for (auto &callback : callbacks) {
                callback(message);
            }
        }
    }
    bool is_dead() {
        return not is_alive;
    }
    void kill() {
        keep_talking = false;
        q.close();
    }
};

#endif
