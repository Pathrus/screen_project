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
    std::cout << "Debut thread sender" << std::endl;

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    portno = 10001;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0)
        std::cerr << "Error opening socket" << std::endl;

    server = gethostbyname("1234");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        std::cerr << "send error connect" << std::endl;

    char buffer[256] = "test";
/*
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) 
         std::cerr << "ERROR writing to socket" << std::endl;
    bzero(buffer,256);
    n = read(sockfd, buffer, 255);
    if (n < 0) 
        std::cerr << "ERROR reading from socket" << std::endl;
    printf("%s\n", buffer);
*/

    
    //sender mon_sender(fif, width, height, fps);
    //mon_sender();
    std::cout << "Fin thread sender" << std::endl;

    return 0;
}