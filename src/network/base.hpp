#ifndef BASE_H
#define BASE_H

#define MAX_CLIENT 30
#define PORT 8888
#define _EOF 0
class base
{
public:
    base(int x);
    int client_sock[MAX_CLIENT];
    char buffer[1025];
    char * MULTICAST_IP; // default;


    //set of socket descriptors
	fd_set readfds;

    int io(int tcpfd, int udpfd, struct sockaddr_in address);
};

#endif
