#include "tcp.hpp"

#ifdef _WIN32
	#include <Winsock2.h>
	#include <WS2tcpip.h>
#else
	#include <unistd.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
#endif

#include <string.h>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

tcp::tcp():udp(){
    int opt = 1;

	tcpfd = socket(AF_INET , SOCK_STREAM , 0);
    if((!validSocket(tcpfd)))
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	
	//set master socket to allow multiple connections ,
	//this is just a good habit, it will work without this
	if( setsockopt(tcpfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
		sizeof(opt)) < 0 )
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	
	//type of socket created
	tcpaddr.sin_family = AF_INET;
	tcpaddr.sin_addr.s_addr = INADDR_ANY;
	tcpaddr.sin_port = htons( 0 );
	
	// print the socket addr
    char str[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET, &(tcpaddr.sin_addr), str, INET_ADDRSTRLEN);
    std::cout << str;

    
	
	//bind the socket to localhost port 8888
	if (bind(tcpfd, (struct sockaddr *)&tcpaddr, sizeof(tcpaddr)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	printf("Listener on port %d \n", PORT);

    char myIP[16];

	struct sockaddr_in my_addr;
    //bzero(&tcpaddr, sizeof(my_addr));
    socklen_t len = sizeof(my_addr);
    getsockname(tcpfd, (struct sockaddr *) &my_addr, &len);
    inet_ntop(AF_INET, &my_addr.sin_addr, myIP, sizeof(myIP));
    int myPort = ntohs(my_addr.sin_port);

    printf("Local ip address: %s\n", myIP);
    printf("Local port : %u\n", myPort);


	if (listen(tcpfd, MAX_CLIENT) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
}