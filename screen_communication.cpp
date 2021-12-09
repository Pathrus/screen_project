#include "headers.h"

//#include "codec.hpp"
#include "sender.hpp"
#include "receiver.hpp"
#include "fifo.hpp"
#include "socket.hpp"

static volatile bool stop = false;
bool running;
fifo fif;

// Client
void thread_sender(int width, int height, AVCodecID id, AVPixelFormat px, int fps)
{
    std::cout << "Debut thread sender" << std::endl;
    std::string ip = "127.0.0.1";
    int port = 1234;
   
/*
    Socket sender(ip, port, "client");
    //sender.binding();
    //char *buffer = "test";
    std::string buffer = "test";
    //sender.sendTo(buffer, sizeof(buffer));
    sender.sendTo(buffer.c_str(), buffer.length());
*/    

/*
    struct sockaddr_in client_addr, serv_addr;
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(port);
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sockfd, reinterpret_cast<sockaddr*>(&client_addr), sizeof(client_addr)) < 0)
        std::cerr << "Erreur bind socket client " << std::endl;
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int ret = sendto(sockfd, "test", 4, 0, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr));
    if(ret < 0)
        std::cerr << "Erreur sendTo" << std::endl;
    std::cout << "send: " << ret << std::endl;
*/

    sender mon_sender(fif, width, height, fps);
    mon_sender();
    std::cout << "Fin thread sender" << std::endl;
}


// server
void thread_receiver(int width, int height, AVCodecID id, AVPixelFormat px, int fps, float ratio)
{
    std::cout << "Debut thread receiver" << std::endl;
    int port = 1234;
    std::string ip = "127.0.0.1";
    //std::string ip = "";

/*    
    Socket receiver(ip, port, "server");
    receiver.binding();
    char buffer[256];
    receiver.recvFrom(buffer, sizeof(buffer));
    std::cout << "recv " << buffer << std::endl;
    //printf("Here is the message: %s\n",buffer);
*/

    /*
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(bind(sockfd, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) < 0)
        std::cerr << "Erreur bind socket server " << std::endl;

    sockaddr_in from;
    socklen_t size = sizeof(from);
    char buffer[256];
    int ret = recvfrom(sockfd, buffer, sizeof(buffer), 0, reinterpret_cast<sockaddr*>(&from), &size);
    std::cout << "recv " << buffer << std::endl;
    */

    receiver mon_receiver(fif, width, height, ratio, fps);
    mon_receiver();
    std::cout << "Fin thread receiver" << std::endl;
}

int main(int argc, char* argv[])
{
    float ratio;
    if(argc > 1)
        ratio = atof(argv[1]);
    else
        ratio = 0.5;
    std::cout << "Début screen_communication - Ctrl+C pour stop" << std::endl;
    AVCodecID codec_id = AV_CODEC_ID_H264;
    AVPixelFormat pix_fmt = AV_PIX_FMT_YUV420P;
    int fps = 60;
    Display* display = XOpenDisplay(nullptr);
    Screen* screen = DefaultScreenOfDisplay(display);
    int width, height;
    width = screen->width - screen->width%2;
    height = screen->height - screen->height%2;

    XCloseDisplay(display);
    running = true;

    std::thread th_rec(&thread_receiver,  width, height, codec_id, pix_fmt, fps, ratio);
    std::thread th_send(&thread_sender, width, height, codec_id, pix_fmt, fps);
    
    //th_send.detach();
    //th_rec.detach();
    
    signal(SIGINT, [](int) { stop = true; });
    while(!stop){}

    running = false;

    th_send.join();
    th_rec.join();

    std::cout << "### FIN COMMU ###" << std::endl;

    return 0;
}
