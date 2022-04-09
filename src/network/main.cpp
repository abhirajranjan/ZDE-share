#include "main.hpp"

Main::Main():tcp(){
}

int Main::run(){
    char str[] = "hello there";
    if(udp::_send(str, strlen(str)) < 0) {
        perror("sendto");
    }
	initialize_myself(tcpfd);
    Main::io(tcpfd, udpfd_recv, tcpaddr);
    return 1;
}

int main(){
    Main obj;
    obj.run();
	while (1);
}

int Main::io(sock_t tcpfd, sock_t udpfd, struct sockaddr_in address) {
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

	tcp::create_threads();

    #ifdef _WIN32
        //
        // Program never actually gets here due to infinite loop that has to be
        // canceled, but since people on the internet wind up using examples
        // they find at random in their own code it's good to show what shutting
        // down cleanly would look like.
        //
        WSACleanup();
    #endif
    return 0;
}


void Main::initialize_myself(sock_t tcpfd) {
	char myIP[16];

	packet::generate_indentity_packet("abhiraj ranjan", tcp_ipaddr, tcp_ipport, "linux", &myself);
	std::cout << myself["os"] << std::endl;
}

