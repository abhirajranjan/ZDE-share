#ifndef BASE_H
#define BASE_H

#define MAX_CLIENT 30
#define PORT 8888
#define _EOF 0

#if defined(_WIN32) || defined(WIN32)
#include <Winsock2.h> // before Windows.h, else Winsock 1 conflict
#include <Windows.h>
#include <Ws2tcpip.h> // needed for ip_mreq definition for multicast
#include <io.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <bits/stdc++.h>
#endif


class base
{
public:

#ifdef _WIN32
    typedef SOCKET sock_t;
    int validSocket(SOCKET sock);
    int pcloseSocket(SOCKET sock);
#else
    typedef int sock_t;
    int validSocket(int sockfd);
    int pcloseSocket(int sockfd);
#endif // _WIN32

    base(int x);
    sock_t client_sock[MAX_CLIENT];
    char buffer[1025];
    char * MULTICAST_IP; // default;
    int max_sd, i, sd, activity, valread;   
  
    //set of socket descriptors
	fd_set readfds;

    int io(sock_t tcpfd, sock_t udpfd, struct sockaddr_in address);
};

#endif
