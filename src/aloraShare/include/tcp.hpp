#include "udp.hpp"

#ifndef TCP_H
#define TCP_H



class tcp: public udp
{
    public:
        tcp();
        void tcp_listen();

        int parse_ping(sock_t fd);
        int parse_text(sock_t fd);
        int parse_file(sock_t fd);
        void handle_client(int pos);

        void create_threads();

        sock_t tcpfd;
        std::string tcp_ipaddr;
        int tcp_ipport;
        struct sockaddr_in tcpaddr;
};

#endif