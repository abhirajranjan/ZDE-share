#include "base.hpp"

#ifndef UDP_H
#define UDP_H


class udp: public base{
    void init();
    void setup_addr(struct sockaddr_in *addr, int port);
    void setup_addr(struct sockaddr_in *addr, int port, std::string *address);

    std::thread sender_thread;
    std::vector<devices> devs;

    enum statuses {blocked, returned, continued};
    enum statuses sender_status;
    // buffer for myself:json to reduce computation every time
    // when sending packets,
    //
    // remember to call updateMyselfBuffer() to regenerate buffer
    //
    char *buffer;

    void sender();
    void recv_udp();
    int _send(char *str);
    void update_devices_list_on_network(json connection_packet);
    void remove_dead_devices(std::vector<devices> *devs);

    public: 
        udp();
        void create_threads();
        void updateMyselfBuffer();
        std::vector<devices> get_devices();

        sock_t udpfd_recv;
        sock_t udpfd_sender;
        struct sockaddr_in udpaddr_recv;
        struct sockaddr_in udpaddr_sender;

};

#endif