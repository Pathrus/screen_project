#include <stdlib.h>
#include "stdio.h"
#include <iostream>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


int main(int argc, char* argv)
{
    std::cout << "Debut server" << std::endl;
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    portno = 10001;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0)
        std::cerr << "Error opening socket" << std::endl;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    //if(bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) <0)
    if(bind(sockfd, reinterpret_cast<sockaddr*> (&serv_addr), sizeof(serv_addr)) <0)
    {
        std::cerr << "Error binding socker" << std::endl;
    }

    listen(sockfd, 5);

    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &clilen);

    if(newsockfd < 0)
    {
        std::cerr << "rec error accept" << std::endl;
    }

    //std::cout << "connection from " << inet_ntoa(cli_addr.sin_addr) << " port " << ntohs(cli_addr.sin_addr) << std::endl;
    char buffer[256];

    //int n = read(newsockfd,buffer,255);
    int ret = recvfrom(sockfd, buffer, sizeof(buffer), 0, reinterpret_cast<sockaddr*>(&cli_addr), &clilen);
    if(ret < 0)
        std::cerr << "rec erreor recv" << std::endl;

    //if (n < 0) std::cerr << "rec ERROR reading from socket" << std::endl;
    printf("Here is the message: %s\n",buffer);

    std::cout << "Fin server" << std::endl;

    return 0;

}