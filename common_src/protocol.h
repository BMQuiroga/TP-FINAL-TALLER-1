#ifndef _PROTOCOL_H
#define _PROTOCOL_H
#include <stdint.h>
#include <arpa/inet.h>
#include <iostream>
#include "socket.h"
#include "matchstate.h"
#include <string>
#include <vector>

// Solicitud del cliente
struct ProtocolRequest {};

// Respuesta que se devuelve al cliente despues de cada solicitud
struct ProtocolResponse {
    // int model_count;
    MatchState state;
};

template <typename Recv_Type>
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
     * Envía un número de n bytes a través
     * del socket
    */
   template <typename T>
    void send_number(
        const T &n,
        Socket &skt,
        bool *was_closed) {
        if (sizeof(n) <= sizeof(uint16_t))
            n = htons(n);
        else
            n = htonl(n);

        skt.sendall(&n, sizeof(n), was_closed);
    }

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
