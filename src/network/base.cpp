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
#endif

#include <string.h>
#include <chrono>
#include <thread>
#include <future>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <bits/stdc++.h>

#include "base.hpp"

#define TRUE 1
#define FALSE 0


base::base(int x){
    MULTICAST_IP = "225.0.0.250";

    std::unordered_map <std::string, std::string> myself;
    myself["name"] = "";
    myself["os"] = "";

    int max_sd, i, sd, activity, valread;

    //initialise all client_socket[] to 0 so not checked
	for (i = 0; i < MAX_CLIENT; i++)
	{
		client_sock[i] = 0;
	}
}


int base::io(int tcpfd, int udpfd, struct sockaddr_in address){
    int max_sd, i, sd, activity, valread, new_socket, addrlen;
    
    addrlen = sizeof(address);

    // now just enter a read-print loop
    while (TRUE)
    {
        //clear the socket set
		FD_ZERO(&readfds);
	
		//add master socket to set
		FD_SET(tcpfd, &readfds);
        FD_SET(udpfd, &readfds);
		max_sd = tcpfd > udpfd ? tcpfd : udpfd;
			
		//add child sockets to set
		for ( i = 0 ; i < MAX_CLIENT ; i++)
		{
			//socket descriptor
			sd = client_sock[i];
				
			//if valid socket descriptor then add to read list
			if(sd > 0)
				FD_SET( sd , &readfds);
				
			//highest file descriptor number, need it for the select function
			if(sd > max_sd)
				max_sd = sd;
		}
	
		//wait for an activity on one of the sockets , timeout is NULL ,
		//so wait indefinitely
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
	
		if ((activity < 0) && (errno!=EINTR))
		{
			printf("select error");
		}

        // something happened on the udp socket
        if(FD_ISSET(udpfd, &readfds))
        {
            if (valread = read(udpfd, buffer, 1024)){
                std::cout << "udp: " << buffer << std::endl;
            }
        }
			
		//If something happened on the master socket ,
		//then its an incoming connection
		if (FD_ISSET(tcpfd, &readfds))
		{
            
			if ((new_socket = accept(tcpfd,
					(struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
			{
				perror("accept");
				exit(EXIT_FAILURE);
			}
			
			//inform user of socket number - used in send and receive commands
			printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs
				(address.sin_port));
		
			//send new connection greeting message
			
            //if( send(new_socket, message, strlen(message), 0) != strlen(message) )
			//{
        	//	perror("send");
			//}
				
			//puts("Welcome message sent successfully");
				
			//add new socket to array of sockets
			for (i = 0; i < MAX_CLIENT; i++)
			{
				//if position is empty
				if( client_sock[i] == 0 )
				{
					client_sock[i] = new_socket;
					printf("Adding to list of sockets as %d\n" , i);
						
					break;
				}
			}
		}
			
		//else its some IO operation on some other socket
		for (i = 0; i < MAX_CLIENT; i++)
		{
			sd = client_sock[i];
				
			if (FD_ISSET( sd , &readfds))
			{
				//Check if it was for closing , and also read the
				//incoming message
				if ((valread = read( sd , buffer, 1024)) == 0)
				{
					//Somebody disconnected , get his details and print
					getpeername(sd , (struct sockaddr*)&address , \
						(socklen_t*)&addrlen);
					printf("Host disconnected , ip %s , port %d \n" ,
						inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
						
					//Close the socket and mark as 0 in list for reuse
					close( sd );
					client_sock[i] = 0;
				}
					
				//Echo back the message that came in
				else
				{
					//set the string terminating NULL byte on the end
					//of the data read
					buffer[valread] = '\0';
					send(sd , buffer , strlen(buffer) , 0 );
				}
			}
		}
    }


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