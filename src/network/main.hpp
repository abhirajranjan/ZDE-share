#include "tcp.hpp"

#ifndef MAIN_H
#define MAIN_H

class Main: public tcp{
    public:
        Main();
        int run();
        void initialize_myself(sock_t tcpfd);
};

#endif