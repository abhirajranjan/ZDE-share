#include "main.hpp"

Main::Main():tcp(){
}

int Main::run(){
    base::io(tcpfd, udpfd, tcpaddr);
    return 1;
}

int main(){
    Main obj;
    return obj.run();
}