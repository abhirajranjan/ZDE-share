#include <iostream>
#include <unordered_map>

#include "base.hpp"

#define TRUE 1
#define FALSE 0

base::base(int x){
    MULTICAST_IP = "225.0.0.250";
    
    //initialise all client_socket[] to 0 so not checked
	for (int i = 0; i < MAX_CLIENT; i++)
	{
		client_sock[i] = 0;
	}
}