#include "main.hpp"
#include "packet.hpp"

Main::Main():tcp(){
}

int Main::run(){
    char str[] = "hello there";
    if(udp::send(str, strlen(str)) < 0) {
        perror("sendto");
    }
	initialize_myself(tcpfd);
    base::io(tcpfd, udpfd_recv, tcpaddr);
    return 1;
}

int main(){
    Main obj;
    return obj.run();
}

int base::io(sock_t tcpfd, sock_t udpfd, struct sockaddr_in address) {
    int max_sd, i, activity, valread, addrlen;
	sock_t sd, new_socket;
    
    addrlen = sizeof(address);

    // now just enter a read-print loop
    while (1)
    {
        //clear the socket set
		FD_ZERO(&readfds);
	
		//add master socket to set
		FD_SET(tcpfd, &readfds);
        FD_SET(udpfd, &readfds);

#ifndef _WIN32
		max_sd = tcpfd > udpfd ? tcpfd : udpfd;
#else
		max_sd = 0;
#endif
		//add child sockets to set
		for ( i = 0 ; i < MAX_CLIENT ; i++)
		{
			//socket descriptor
			sd = client_sock[i];
				
			//if valid socket descriptor then add to read list

			if(validSocket(sd) && (sd > 0))
				FD_SET(sd , &readfds);

#ifndef _WIN32
			//highest file descriptor number, need it for the select function
			if(sd > max_sd)
				max_sd = sd;
#endif
		}
	
		//wait for an activity on one of the sockets , timeout is NULL ,
		//so wait indefinitely
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
	
#ifdef _WIN32
		if ((activity == SOCKET_ERROR) && (WSAGetLastError() == WSANOTINITIALISED || // a successful WSAStartup call must occur before
													WSAEFAULT || // windows socket was unable to allocate needed resources for internal operations
													WSAENETDOWN || // network subsystem failed 
													WSAEINVAL || // time out value is not valid or all three readfd, writefs, execptfs are NULL
													WSAEINTR || // blocking windows socket 1.1 call was canceled through WSACancelBlockingCall
													WSAEINPROGRESS || //blocking windows socket 1.1 call in progress
													WSAENOTSOCK // one of th descriptor sets containing an entry that is not a socket
			)){
				perror("select socket error");
				exit(EXIT_FAILURE);
			}
#else
		if ((activity < 0) && (errno!=EINTR))
			{
				printf("select error");
			}
#endif

        // something happened on the udp socket
        if(FD_ISSET(udpfd, &readfds))
        {
            if ((valread = recv(udpfd, udp_buffer, 1024, 0))){
				_udp_buffer = udp_buffer;

				std::cout << "udp recv: " << _udp_buffer << std::endl;

				//check if recieved packet is json or not
				// if yes then pass on to parser.
				if(json::accept(_udp_buffer)){
					packet::parse_packet(_udp_buffer);
				}
            }
        }
			
		//If something happened on the tcp socket ,
		//then its an incoming connection
		if (FD_ISSET(tcpfd, &readfds))
		{
			if ((new_socket = accept(tcpfd,
					(struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
			{
				perror("accept");
				exit(EXIT_FAILURE);
			}
			
			//inform user of socket number - used in send and receive commands
			printf("New connection, ip is : %s , port : %d\n", inet_ntoa(address.sin_addr) , ntohs
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
				if( client_sock[i] == 0)
				{
					client_sock[i] = new_socket;
					printf("Adding to list of sockets as %d\n" , i);
					std::cout << client_sock;
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

				if ((valread = recv(sd, buffer, 1024, 0)) == 0)
				{
					//Somebody disconnected , get his details and print
					getpeername(sd , (struct sockaddr*)&address , (socklen_t*) &addrlen);
					printf("Host disconnected , ip %s , port %d \n" ,
						inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
						
					//Close the socket and mark as 0 in list for reuse
					pcloseSocket(sd);
					client_sock[i] = 0;
				}
					
				//Echo back the message that came in
				else
				{
					//set the string terminating NULL byte on the end
					//of the data read
					buffer[valread] = '\0';
					send(sd , buffer , (int) strlen(buffer) , 0 );
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
return 0;
}


void Main::initialize_myself(sock_t tcpfd) {
	char myIP[16];

	packet::generate_indentity_packet("abhiraj ranjan", tcp_ipaddr, tcp_ipport, "linux", &myself);
	std::cout << myself["os"] << std::endl;
}

