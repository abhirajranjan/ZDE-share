#include <arpa/inet.h>
#include <iostream>

int PORT = 8888;
char _EOF = 0;
char * MULTICAST_IP = "225.0.0.250"; // default;
const int MAX_CLIENT = 30;

// struct sockaddr_in sa;
//inet_pton(AF_INET, "192.0.2.33", &(sa.sin_addr));
