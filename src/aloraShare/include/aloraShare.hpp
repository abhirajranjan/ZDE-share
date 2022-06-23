#include "tcp.hpp"

#ifndef MAIN_H
#define MAIN_H

class aloraShare: public tcp{
    int io(sock_t tcpfd, sock_t udpfd, struct sockaddr_in address);
    
    public:
        aloraShare();
        int run();
        void update_identity(std::string name);

};

#endif