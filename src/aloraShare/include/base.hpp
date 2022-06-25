#ifndef BASE_H
#define BASE_H

#define MAX_CLIENT 30
#define PORT 8888
#define _EOF 0
#define PCKT_SND_AFTR 3 // in sec
#define UDP_CONNECTION_LIST_REFRESH 12


#include "platform.hpp"
#include "packet.hpp"
#include "sockets.hpp"


struct devices
{
    std::time_t time;
    json info;
};

class base
{
    public:
        int validSocket(sock_t sockfd);
        int pcloseSocket(sock_t sockfd);

        base(int x);
        void initialize_myself(sock_t tcpfd);

        json myself;

        sock_t client_sock[MAX_CLIENT];
        
        std::thread thread_array[MAX_CLIENT];

        std::string MULTICAST_IP; // default;
        int max_sd, sd, activity, valread;   
};

#endif
