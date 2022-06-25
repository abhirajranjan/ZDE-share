#include "base.hpp"

#ifndef UDP_H
#define UDP_H


class udp: public base
{
    void init();

    std::thread sender_thread;
    std::vector<devices> devs;

    enum statuses { blocked, returned, continued };
    enum statuses sender_status;
    
    void sender();
    void recv_udp();
    int _send(const char *str);
    void update_devices_list_on_network(json connection_packet);
    void remove_dead_devices(std::vector<devices> *devs);

    public: 
        udp();
        void create_threads();
        std::vector<devices> get_devices();

        sock_t udpfd_recv;
        sock_t udpfd_sender;
        struct sockaddr_in udpaddr_recv;
        struct sockaddr_in udpaddr_sender;

};

#endif