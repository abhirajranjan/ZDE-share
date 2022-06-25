#include "tcp.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


std::string getlocalip(){
    const char* google_dns_server = "8.8.8.8";
    int dns_port = 53;

    struct sockaddr_in serv;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        std::cout << "Socket error" << std::endl;
    }

    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(google_dns_server);
    serv.sin_port = htons(dns_port);

    int err = connect(sock, (const struct sockaddr*)&serv, sizeof(serv));
    if (err < 0)
    {
        std::cout << "Error number: " << errno
            << ". Error message: " << strerror(errno) << std::endl;
    }

    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);
    err = getsockname(sock, (struct sockaddr*)&name, &namelen);

    char buffer[80];
    const char* p = inet_ntop(AF_INET, &name.sin_addr, buffer, 80);
    if(p != NULL)
    {
        std::string str = buffer;
        return str;
    }
    else
    {
        perror("could not determine hostname: ");
        exit(EXIT_FAILURE);
    }
}

tcp::tcp():udp()
{
    tcpfd = networking::generate_socket(AF_INET, SOCK_STREAM, 0);
	
    networking::setup_addr(AF_INET, 0, &tcpaddr);
    
	// print the socket addr
    char str[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET, &(tcpaddr.sin_addr), str, INET_ADDRSTRLEN);
    std::cout << str << std::endl;
    
	networking::bind_socket(tcpfd, (struct sockaddr *)&tcpaddr, sizeof(tcpaddr));

    char myIP[16];

	struct sockaddr_in my_addr;
    //bzero(&tcpaddr, sizeof(my_addr));
    socklen_t len = sizeof(my_addr);
    getsockname(tcpfd, (struct sockaddr *) &my_addr, &len);
    inet_ntop(AF_INET, &my_addr.sin_addr, myIP, sizeof(myIP));
    int myPort = ntohs(my_addr.sin_port);

    //printf("Local ip address: %s\n", myIP);
    printf("\nLocal port : %u\n", myPort);

	tcp_ipport = myPort;
	tcp_ipaddr = getlocalip();

	if (listen(tcpfd, MAX_CLIENT) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
}

void tcp::handle_client(int pos)
{
    char buffer[1025];
    std::string string_buffer;
    
    sock_t fd = client_sock[pos];

    //recieve headers
    if ((valread = recv(fd, buffer, 1024, 0)))
    {
            string_buffer = buffer;
    }

    //check if headers is json
    // if not close connection and free socket 
    if(!json::accept(string_buffer))
    {
        networking::pcloseSocket(fd);
        client_sock[pos] = 0;
        return;
    }

    json j = json::parse(string_buffer);

    // validate json
    if (!packet::validate_packet(j))
    {
        networking::pcloseSocket(fd);
        client_sock[pos] = 0;
        return;
    }

    switch ((int) j["pckt"])
    {
    case packet::ping_transfer:
        parse_ping(fd);
        break;

    case packet::text_transfer:
        parse_text(fd);
        break;

    case packet::file_transfer:
        parse_file(fd);
        break;

    // non recognizable tcp packet id
    default:
        networking::pcloseSocket(fd);
        client_sock[pos] = 0;
        return;
    }
}

int tcp::parse_ping(int fd)
{
    char c_buffer[1024];

    if ((valread = recv(fd, c_buffer, 1024, 0)))
    {
        std::cout << "ping: " << c_buffer << std::endl;
        return 1;
    }
    return 0;
}

int tcp::parse_text(int fd)
{
    return 1;

}

int tcp::parse_file(int fd)
{
    return 1;
}

void tcp::tcp_listen()
{
    int valread, addrlen =  sizeof(tcpaddr);
	sock_t new_socket;

    while(1)
    {
        if ((new_socket = accept(tcpfd,(struct sockaddr *)&tcpaddr, (socklen_t*)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        
        //inform user of socket number - used in send and receive commands
        printf("New connection, ip is : %s , port : %d\n", inet_ntoa(tcpaddr.sin_addr) , ntohs(tcpaddr.sin_port));

        //add new socket to array of sockets
        for (int i = 0; i < MAX_CLIENT; i++)
        {
            //if position is empty
            if( client_sock[i] == 0)
            {
                //create and start a seperate thread for communication 
                thread_array[i] = std::thread([this, i](){
                    tcp::handle_client(i);
                });
                thread_array[i].detach();

                client_sock[i] = new_socket;
                printf("Adding to list of sockets as %d\n" , i);
                break;
            }
        }
    }
}

void tcp::create_threads()
{
    udp::create_threads();
    
    std::thread listen_thread([this](){
        tcp::tcp_listen();
    });

    listen_thread.detach();
}