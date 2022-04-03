#include <arpa/inet.h>

#include "base.hpp"

#ifndef UDP_H
#define UDP_H

class udp: public base{
    public: 
        udp();

        int udpfd;
        struct sockaddr_in udpaddr;

        void sendIdentity();
};

#endif