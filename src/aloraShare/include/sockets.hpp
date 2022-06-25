#include "platform.hpp"

namespace networking
{
    #ifdef _WIN32
    inline int validSocket(SOCKET sock)
    {
        if (sock == INVALID_SOCKET)
        {
            return 0;
        }
        return 1;
    }

    inline int pcloseSocket(SOCKET sock)
    {
        if(base::validSocket(sock))
        {
            return closesocket(sock);
        }
        return 1;
    }
    #else
    inline int validSocket(int sock)
    {
        if (sock > 0)
        {
            return 1;
        }
        return 0;
    }

    inline int pcloseSocket(int sock)
    {
        if(validSocket(sock))
        {
            return close(sock);
        }
        return 1;
    }
    #endif

    inline void setup_addr(int protocol, int port, struct sockaddr_in *addr)
    {
        memset(addr, 0, sizeof(*addr)); 
        addr -> sin_family = protocol;
        addr -> sin_addr.s_addr = htonl(INADDR_ANY); // differs from sender
        addr -> sin_port = htons(port);
    }

    inline void setup_addr(int protocol, int port, std::string *address, struct sockaddr_in *addr)
    {
        memset(addr, 0, sizeof(*addr));
        addr -> sin_family = protocol;
        addr -> sin_addr.s_addr = inet_addr(address->c_str());
        addr -> sin_port = htons(port);
    }

    inline sock_t generate_socket(int __domain, int __type, int __protocol)
    {
        sock_t sock = socket(__domain, __type, __protocol);
        
        if (!validSocket(sock))
        {
            perror("error creating network socket");
            exit(EXIT_FAILURE);
        }

        u_int yes = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*) &yes, sizeof(yes)) < 0)
        {
            perror("Reusing address failed");
            exit(EXIT_FAILURE);
        }

        return sock;
    }

    inline bool bind_socket(sock_t __fd, const sockaddr *__addr, socklen_t __len)
    {
        if (bind(__fd, __addr, __len) < 0)
        {
            perror("socket bind failed");
            exit(EXIT_FAILURE);
        }
        return true;
    }
}