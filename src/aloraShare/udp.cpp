#include "udp.hpp"

#include <string.h>
#include <chrono>
#include <thread>
#include <future>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <ctime>
 

#define MSGBUFSIZE 256
#define TRUE 1
#define FALSE 0


void udp::setup_addr(struct sockaddr_in *addr, int port) {
    // set up destination address
    memset(addr, 0, sizeof(*addr));
    addr -> sin_family = AF_INET;
    addr -> sin_addr.s_addr = htonl(INADDR_ANY); // differs from sender
    addr -> sin_port = htons(port);
}

void udp::setup_addr(struct sockaddr_in *addr, int port, std::string *address) {
    // set up destination address
    memset(addr, 0, sizeof(*addr));
    addr -> sin_family = AF_INET;
    addr -> sin_addr.s_addr = inet_addr(address->c_str()); // differs from sender
    addr -> sin_port = htons(port);
}

int udp::_send(const char* str){
    // main send function to send packets in udp
    //
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
    setup_addr(&udpaddr_sender, PORT, &MULTICAST_IP);

    // bind udp_recv to multicast addr address
    //
    if (bind(udpfd_recv, (struct sockaddr*) &udpaddr_recv, sizeof(udpaddr_recv)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // use setsockopt() to request that the kernel join a multicast group
    //
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_IP.c_str());
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
    // send buffer :char* to network 
    //
    while (udp::sender_status != returned){
        udp::_send(myself.dump().c_str());
        sleep(PCKT_SND_AFTR);
    }
}

void udp::recv_udp(){
    // recv packets in network
    //
    char udp_buffer[1025];
    std::string _udp_buffer;

    while (1){
        memset(udp_buffer, 0, 1025);
        if ((valread = recv(udpfd_recv, udp_buffer, 1024, 0))){
            _udp_buffer = udp_buffer;

            //check if recieved packet is json or not
            // if yes then pass on to parser.
            //
            if(json::accept(_udp_buffer)){
                packet::resp response = packet::udp_parse_packet(_udp_buffer);
                if (response.type != packet::empty){
                    if (
                        // ip 	port 	result  (1 = same, 0 = diff)
                        // 0	0	    0
                        // 0	1	    0
                        // 1	0	    0
                        // 1	1	    1
                        //
                        response.data["ip"] == myself["ip"] &
                        response.data["port"] == myself["port"]
                    ){
                        // our own packet
                        //
                        continue;
                    }
                    switch (response.type){
                        case packet::identity:
                            udp::update_devices_list_on_network(response.data);
                            break;
                        case packet::file_transfer:
                            break;
                        case packet::ping_transfer:
                            break;
                        case packet::text_transfer:
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }
}

void udp::create_threads(){
    sender_status = continued;
    std::thread sender_thread([this](){
        udp::sender();
    });

    std::thread recv_thread([this](){
        udp::recv_udp();
    });
    
    sender_thread.detach();
    recv_thread.detach();
}

udp::udp():base(2) {
    init();
}

// connection functions

std::time_t time(){
    std::time_t ctime = std::time(nullptr);
    return ctime;
}

bool comp_devices(json dev1, json dev2){
    std::string dev1_ip = (std::string) dev1["ip"];
    std::string dev2_ip = (std::string) dev2["ip"];

    int dev1_port = (int) dev1["port"];
    int dev2_port = (int) dev2["port"];

    return ((dev1_ip == dev2_ip) & (dev1_port == dev2_port)) ? true : false;
}

void udp::update_devices_list_on_network(json connection_packet){
        
        for(auto device : devs){
            if(comp_devices(connection_packet, device.info)){
                device.time = time();
                return;
            }
        }

        devices dev = {time(), connection_packet};
        devs.push_back(dev);
        return;
}

void udp::remove_dead_devices(std::vector<devices> *devs){
    std::time_t ctime = time();
    

    for(std::vector<devices>::iterator iter = devs->begin(); iter != devs->end();){
        if(ctime - iter->time > UDP_CONNECTION_LIST_REFRESH){
            iter  = devs->erase(iter);
        }
        else
            ++iter;
    }
}

std::vector<devices> udp::get_devices(){
    udp::remove_dead_devices(&devs);
    return devs;
}
