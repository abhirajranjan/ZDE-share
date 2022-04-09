#include "tcp.hpp"

#ifndef MAIN_H
#define MAIN_H

class Main: public tcp{
    int io(sock_t tcpfd, sock_t udpfd, struct sockaddr_in address);
    
    public:
        Main();
        int run();
        void initialize_myself(sock_t tcpfd);

};

#endif