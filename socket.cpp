#include "socket.hpp"

Socket::Socket(std::string ip_add, int portno, std::string mode): ip(ip_add), port(portno)
{
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sockfd < 0)
        std::cerr << "Error opening socket" << std::endl; // avec throw ?   
    
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(portno);
    
    if(mode == "client")
    {
        my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        to_addr.sin_addr.s_addr = inet_addr(ip_add.c_str());
        to_addr.sin_family = AF_INET;
        to_addr.sin_port = htons(portno);
    }

    if(mode == "server")
        my_addr.sin_addr.s_addr = inet_addr(ip_add.c_str());
}

int Socket::binding()
{
    if(bind(sockfd, reinterpret_cast<sockaddr*>(&my_addr), sizeof(my_addr)) < 0)
    {
        std::cerr << "Erreur bind socket, " << ip << std::endl;
        return -1;
    }
    return 0;
}
    
int Socket::listening()
{
    if(listen(sockfd, 20) < 0)
    {
        std::cerr << "Erreur listen" << std::endl;
        return -1;
    }
    return 0;
}

int Socket::accepting()
{
    socklen_t len = sizeof(to_addr);
    if(accept(sockfd, reinterpret_cast<sockaddr*>(&to_addr), &len) < 0)
    {
        std::cerr << "Erreur accept connection" << std::endl;
        return -1;
    }
    return 0;
}

int Socket::connecting()
{
    if(connect(sockfd, reinterpret_cast<sockaddr*>(&to_addr), sizeof(to_addr)) < 0)
    {
        std::cerr << "Erreur connection" << std::endl;
        return -1;
    }
    //std::cout << "success connection" << std::endl;
    return 0;
}


void Socket::sendTo(const char* buffer, int size)
{
    //int ret = sendto(sockfd, buffer, size, 0, reinterpret_cast<sockaddr*>(&to_addr), sizeof(to_addr));
    int ret = sendto(sockfd, buffer, size, 0, reinterpret_cast<sockaddr*>(&to_addr), sizeof(to_addr));
    if(ret < 0)
        std::cerr << "Erreur sendTo" << std::endl;
    //std::cout << "Send " << ret << " size " << size << std::endl;
}

int Socket::recvFrom(char* buffer, int len)
{
    //sockaddr_in from;
    socklen_t size = sizeof(to_addr);
    int ret = recvfrom(sockfd, buffer, len, 0, reinterpret_cast<sockaddr*>(&to_addr), &size);
    if(ret < 0)
    {
        std::cerr << "Erreur recvFrom" << std::endl;
        return ret;
    }
    //recv(sockfd, buffer, len, 0);
    std::cout << "recv " << ret << std::endl;
    return ret;
}