#ifndef _PROTOCOL_H
#define _PROTOCOL_H
#include <stdint.h>
#include <arpa/inet.h>
#include <iostream>
#include "socket.h"
#include "queue.h"
#include "matchstate.h"
#include <string>
#include <vector>
#include <algorithm>
#include "protocol_types.h"

#define MOVE_LEFT 0
#define MOVE_RIGHT 1
#define MOVE_UP 2
#define MOVE_DOWN 3
#define STOP_MOVING_LEFT 4
#define STOP_MOVING_RIGHT 5
#define STOP_MOVING_UP 6
#define STOP_MOVING_DOWN 7
#define SHOOT 8
#define STOP_SHOOTING 9
#define JOIN 10
#define CREATE 11
#define LIST 12
#define PLAYERNAME 13
#define RELOAD 14
#define PREPARE_GRENADE 15
#define THROW_GRENADE 16
#define INVALID -1

template <typename Send_Type, typename Recv_Type>
class Protocol {
    private:
    const std::string hostname;
    
    public:
    Protocol() = default;

    /**
     * Recibe datos a traves del socket skt y los guarda en un 
     * objeto del tipo Recv_Type.
    */
    virtual Recv_Type get(
        Socket &skt,
        bool *was_closed) = 0;

    /**
     * Envía datos del tipo Send_Type a traves del socket skt
    */
    virtual void send(
        Socket &skt,
        const Send_Type &message,
        bool was_closed) = 0;

    /**
     * Envía un número de n bytes a través
     * del socket
    */
   template <typename T>
    int send_number(
        T n,
        Socket &skt,
        bool *was_closed) {
        if (sizeof(n) == sizeof(uint16_t))
            n = htons(n);
        else if (sizeof(n) == sizeof(uint32_t))
            n = htonl(n);

        return skt.sendall(&n, sizeof(n), was_closed);
    }

    /**
     * Recibe un número de n bytes a través
     * del socket
    */
   template <typename T>
    int receive_number(
        T *n,
        Socket &skt,
        bool *was_closed) {
        int bytes = skt.recvall(n, sizeof(*n), was_closed); 
        if (sizeof(*n) == sizeof(uint16_t))
            *n = ntohs(*n);
        else if (sizeof(*n) == sizeof(uint32_t))
            *n = ntohl(*n);

        return bytes;
    }

    /**
     * Envia un mensaje de texto a traves del 
     * socket
    */
    /*std::string send_text_message(
        std::string &message,
        Socket &skt,
        bool *was_closed) {
        std::vector<char> buf;
        size_t size = message.size();
        std::copy_if(message.begin(), message.end(), std::back_inserter(buf), [](char c) { return c != '\0'; });
        skt.sendall(&size, sizeof(size), was_closed);
        skt.sendall(buf.data(), buf.size(), was_closed);
    }*/

    /**
     * Recibe el texto del mensaje recibido para 
     * un comando de broadcast
    */
    std::string receive_text_message(
        Socket &peer,
        bool *was_closed) {
        uint16_t size;

        // get the string size
        peer.recvall(&size, sizeof(size), was_closed);
        size = ntohs(size);

        // store text bytes in a buffer and return a newly constructed string
        std::vector<char> buffer(size);
        peer.recvall(buffer.data(), buffer.size(), was_closed);
        std::string message(buffer.begin(), buffer.end());
        return message;
    }
    
    /**
     * Recibe un número de 2 o 4 bytes a través
     * del socket
    */
    template<typename T>
    T recv_number(
        Socket &peer, char *buffer, size_t size, bool *was_closed) {
        peer.recvall(buffer, size, was_closed);
        if constexpr(std::is_same_v<T, uint16_t>) {
            return ntohs(*(T*)buffer);
        } else {
            return ntohl(*(T*)buffer);
        }
    }

    /*
     * No queremos permitir que alguien haga copias
     * */
    Protocol(const Protocol&) = delete;
    Protocol& operator=(const Protocol&) = delete;

    Protocol(Protocol&&) = default;
    Protocol& operator=(Protocol&&) = default;
};
#endif
