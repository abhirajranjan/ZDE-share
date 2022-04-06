#include "main.hpp"

Main::Main():tcp(){
}

int Main::run(){
    char *str = "hello there";
    if(udp::send(str) < 0) {
        perror("sendto");
    }
    base::io(tcpfd, udpfd_recv, tcpaddr);
    return 1;
}

int main(){
    Main obj;
    return obj.run();
}