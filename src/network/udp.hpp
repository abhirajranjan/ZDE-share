#include "base.hpp"

#ifndef UDP_H
#define UDP_H


class udp: public base{
    void init();
    void setup_addr(struct sockaddr_in *addr, int port);
    void setup_addr(struct sockaddr_in *addr, int port, char *address);

    enum statuses {blocked, returned, continued};
    enum statuses sender_status;

    void sender();

    public: 
        udp();

        sock_t udpfd_recv;
        sock_t udpfd_sender;
        struct sockaddr_in udpaddr_recv;
        struct sockaddr_in udpaddr_sender;

        int send(char *str, int length);
};

#endif