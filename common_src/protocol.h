#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <stdint.h>
#include <arpa/inet.h>
#include <iostream>
#include "socket.h"
#include "../server_src/matchstate.h"
#include <string>
#include <vector>
#include <sstream>
#include <tuple>

enum CommandType {
    CREATE = 0x01,
    JOIN = 0x02,
    BROADCAST = 0x03,
    BROADCASTED = 0x04,
    INVALID = -1
};


class Protocol {
    private:
    const std::string hostname;
    
    public:
    Protocol() = default;
    
    /**
     * Envía un número de 4 bytes a través
     * del socket
    */
    void send_number(
        const uint32_t &n,
        Socket &skt,
        bool *was_closed);
    /**
     * Envía un número de 2 bytes a través
     * del socket
    */
    void send_number(
        const uint16_t &n,
        Socket &skt,
        bool *was_closed);

    /**
     * Recibe el texto del mensaje recibido para 
     * un comando de broadcast
    */
    std::string receive_text_message(
        Socket &peer,
        size_t size, 
        bool *was_closed);
    
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
