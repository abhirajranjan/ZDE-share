#include "base.hpp"

#ifndef UDP_H
#define UDP_H


class udp: public base{
    void windowsInit();
    void unixInit();
    void setup_addr();

    public: 
        udp();

        sock_t udpfd;
        struct sockaddr_in udpaddr;
};

#endif