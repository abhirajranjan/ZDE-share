#include <iostream>
#include <unordered_map>

#include "base.hpp"

#define TRUE 1
#define FALSE 0

#ifdef _WIN32
int base::validSocket(SOCKET sock) {
	if (sock == INVALID_SOCKET) {
		return 0;
	}
	return 1;
}

int base::pcloseSocket(SOCKET sock){
	if(base::validSocket(sock)){
		return closesocket(sock);
	}
	return 1;
}
#else
int base::validSocket(int sock) {
	if (sock > 0) {
		return 1;
	}
	return 0;
}

int base::pcloseSocket(int sock){
	if(base::validSocket(sock)){
		return close(sock);
	}
	return 1;
}
#endif

base::base(int x){
    MULTICAST_IP = "225.0.0.250";
    
    //initialise all client_socket[] to 0 so not checked
	for (int i = 0; i < MAX_CLIENT; i++)
	{
		client_sock[i] = 0;
	}
}