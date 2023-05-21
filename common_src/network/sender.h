#ifndef _SENDER_H
#define _SENDER_H

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
#include "../socket.h"
#include "../queue.h"
#include "../thread.h"
#include "../protocol.h"

template <typename Send_Type, typename Recv_Type>
class Sender:public Thread {
private:
    Socket& skt;
    Queue<Send_Type>& q;
    Protocol<Send_Type, Recv_Type>& protocol;
    std::atomic<bool> keep_talking{true};
    std::atomic<bool> is_alive{true};
    bool is_queue_blocking;

public:
    explicit Sender(Socket& socket,
        Queue<Send_Type>& q,
        Protocol<Send_Type, Recv_Type>& protocol,
        bool is_queue_blocking = true) :
    skt(socket),
    q(q),
    protocol(protocol),
    is_queue_blocking(is_queue_blocking) {}

    void run() override {
        while (keep_talking) {
            Send_Type message;
            if (is_queue_blocking) {
                message = q.pop();
            } else {
                q.try_pop(message);
            }
            protocol.send(skt, message, keep_talking);
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
