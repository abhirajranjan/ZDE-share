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

void udp::setup_addr(struct sockaddr_in *addr, int port) {
    // set up destination address
    memset(addr, 0, sizeof(*addr));
    addr -> sin_family = AF_INET;
    addr -> sin_addr.s_addr = htonl(INADDR_ANY); // differs from sender
    addr -> sin_port = htons(port);
}

void udp::setup_addr(struct sockaddr_in *addr, int port, char *address) {
    // set up destination address
    memset(addr, 0, sizeof(*addr));
    addr -> sin_family = AF_INET;
    addr -> sin_addr.s_addr = inet_addr(address); // differs from sender
    addr -> sin_port = htons(port);
}

int udp::send(char* str){
    return sendto(
        udpfd_sender,
        str,
        strlen(str),
        0,
        (struct sockaddr *) &udpaddr_sender,
        sizeof(udpaddr_sender)
    );
}

void udp::init() {
    udpfd_recv = socket(AF_INET, SOCK_DGRAM, 0);
    udpfd_sender = socket(AF_INET, SOCK_DGRAM, 0);

    // check for sockets
    //
    if ((!base::validSocket(udpfd_recv)) && (!base::validSocket(udpfd_sender))) {
        perror("udp socket");
        exit(EXIT_FAILURE);
    }

    // allow multiple sockets to use the same PORT number
    //
    u_int yes = 1;
    if (
        setsockopt(
            udpfd_recv, SOL_SOCKET, SO_REUSEADDR, (char*) &yes, sizeof(yes)
        ) < 0
    ){
       perror("Reusing ADDR failed");
       exit(EXIT_FAILURE);
    }
    
    if (
        setsockopt(
            udpfd_sender, SOL_SOCKET, SO_REUSEADDR, (char*) &yes, sizeof(yes)
        ) < 0
    ){
       perror("Reusing ADDR failed");
       exit(EXIT_FAILURE);
    }

    setup_addr(&udpaddr_recv, PORT);
    setup_addr(&udpaddr_sender, PORT, MULTICAST_IP);

    // bind udp_recv to multicast addr address
    //
    if (bind(udpfd_recv, (struct sockaddr*) &udpaddr_recv, sizeof(udpaddr_recv)) < 0) {
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
            udpfd_recv, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq)
        ) < 0
    ){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
}

void udp::sender(){
    while (udp::sender_status != returned){
        // send datagram myself
        udp::send("hello guys\n\0"); 
        sleep(PCKT_SND_AFTR);
    }
}

udp::udp():base(2) {
    initWinSocket();
    init();

    sender_status = continued;
    std::thread sender_thread([this](){
        udp::sender();
    });
    sender_thread.detach();
}