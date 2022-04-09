#ifndef BASE_H
#define BASE_H

#define MAX_CLIENT 30
#define PORT 8888
#define _EOF 0
#define PCKT_SND_AFTR 1 // in sec

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

#include "../json/single_include/nlohmann/json.hpp"
using json = nlohmann::json;


class base
{
public:

#ifdef _WIN32
    typedef SOCKET sock_t;
#else
    typedef int sock_t;
#endif // _WIN32

    int validSocket(sock_t sockfd);
    int pcloseSocket(sock_t sockfd);

    base(int x);
    void initialize_myself(sock_t tcpfd);

    json myself;
    char buffer[1025];

    sock_t client_sock[MAX_CLIENT];

    char udp_buffer[1025];
    std::string _udp_buffer;
    
    char client_buffer[MAX_CLIENT][1025];
    

    char* MULTICAST_IP; // default;
    int max_sd, i, sd, activity, valread;   
  
    //set of socket descriptors
	fd_set readfds;

    int io(sock_t tcpfd, sock_t udpfd, struct sockaddr_in address);
};

#endif
