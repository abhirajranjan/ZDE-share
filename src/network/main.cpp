#include "main.hpp"

Main::Main():tcp(){
}

int Main::run(){
    base::io(tcpfd, udpfd, tcpaddr);
}

int main(){
    Main obj;
    obj.run();
    return 1;
}