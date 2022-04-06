#include "udp.hpp"

#ifndef TCP_H
#define TCP_H


class tcp: public udp{
    public:
        tcp();

        sock_t tcpfd;
        struct sockaddr_in tcpaddr;
};

#endif