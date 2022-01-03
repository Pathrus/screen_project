#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <iostream>

#ifndef SOCKET_HPP
#define SOCKET_HPP

class Socket
{
    public :
        Socket(std::string ip, int portno, std::string mode="");
        int binding();
        int listening();
        int accepting();
        int connecting();
        void sendTo(const char* buffer, int size);
        int recvFrom(char* buffer, int size);

    private:
        std::string ip;
        int port;
        int sockfd;
        struct sockaddr_in my_addr;
        struct sockaddr_in to_addr;
};

#endif