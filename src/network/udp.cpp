#ifdef _WIN32
    #include <Winsock2.h> // before Windows.h, else Winsock 1 conflict
    #include <Ws2tcpip.h> // needed for ip_mreq definition for multicast
    #include <Windows.h>
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <time.h>
    #include <unistd.h>
#endif

#include <string.h>
#include <chrono>
#include <thread>
#include <future>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
 

#include "udp.hpp"

#define MSGBUFSIZE 256
#define TRUE 1
#define FALSE 0

void initWinSocket() {
#ifdef _WIN32
    //
    // Initialize Windows Socket API with given VERSION.
    //
    WSADATA wsaData;
    int result = WSAStartup(0x0101, &wsaData);
    if (result) {
        perror("WSAStartup");
        exit(EXIT_FAILURE);
    }
#endif
}

void udp::setup_addr() {
    // set up destination address
//
    memset(&udpaddr, 0, sizeof(udpaddr));
    udpaddr.sin_family = AF_INET;
    udpaddr.sin_addr.s_addr = htonl(INADDR_ANY); // differs from sender
    udpaddr.sin_port = htons(PORT);
}

/*void udp::windowsInit() {
#ifdef _WIN32
    initWinSocket();
    udpfd = socket(AF_INET, SOCK_STREAM, 0);
    if (udpfd == INVALID_SOCKET) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // allow multiple sockets to use the same PORT number
    //
    u_int yes = 1;
    if (setsockopt(
            udpfd, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(yes)
        ) < 0) {
        perror("Reusing ADDR failed");
        exit(EXIT_FAILURE);
    }

    setup_addr();
    
    // bind to receive address
    //
    if (bind(udpfd, (struct sockaddr*)&udpaddr, sizeof(udpaddr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // use setsockopt() to request that the kernel join a multicast group
    //
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_IP);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (
        setsockopt(
            udpfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq)
        ) < 0
        ) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

#endif
}
*/

void udp::unixInit() {
    udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (!base::validSocket(udpfd)) {
        perror("udp socket");
        exit(EXIT_FAILURE);
    }
    // allow multiple sockets to use the same PORT number
    u_int yes = 1;
    if (
        setsockopt(
            udpfd, SOL_SOCKET, SO_REUSEADDR, (char*) &yes, sizeof(yes)
        ) < 0
    ){
       perror("Reusing ADDR failed");
       exit(EXIT_FAILURE);
    }
    
    setup_addr();

    // bind to receive address
    //
    if (bind(udpfd, (struct sockaddr*) &udpaddr, sizeof(udpaddr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // use setsockopt() to request that the kernel join a multicast group
    //
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_IP);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (
        setsockopt(
            udpfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq)
        ) < 0
    ){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
}


udp::udp():base(2) {
    initWinSocket();
    unixInit();
}
