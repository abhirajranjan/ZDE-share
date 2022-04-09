#include "udp.hpp"

#ifndef TCP_H
#define TCP_H


class tcp: public udp{
    public:
        tcp();

        sock_t tcpfd;
        std::string tcp_ipaddr;
        int tcp_ipport;
        struct sockaddr_in tcpaddr;
};

#endif