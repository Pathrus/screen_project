#include "headers.h"

#include "codec.hpp"
#include "sender.hpp"
#include "receiver.hpp"
#include "fifo.hpp"

static volatile bool stop = false;
bool running;
fifo fif;

void thread_sender(int width, int height, AVCodecID id, AVPixelFormat px, int fps)
{
    std::cout << "Debut thread sender" << std::endl;
    sender mon_sender(fif, width, height, id, px, fps);
    mon_sender();
    std::cout << "Fin thread sender" << std::endl;
}

void thread_receiver(int width, int height, AVCodecID id, AVPixelFormat px, int fps, float ratio)
{
    std::cout << "Debut thread receiver" << std::endl;
    receiver mon_receiver(fif, width, height, ratio, id, px, fps);
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
    std::cout << "Début exo2 - Ctrl+C pour stop" << std::endl;
    AVCodecID codec_id = AV_CODEC_ID_H264;
    AVPixelFormat pix_fmt = AV_PIX_FMT_YUV444P;
    int fps = 60;
    Display* display = XOpenDisplay(nullptr);
    Screen* screen = DefaultScreenOfDisplay(display);
    int width, height;
    width = screen->width;
    height = screen->height;

    XCloseDisplay(display);
    running = true;

    std::thread th_send(&thread_sender, width, height, codec_id, pix_fmt, fps);
    std::thread th_rec(&thread_receiver,  width, height, codec_id, pix_fmt, fps, ratio);
    signal(SIGINT, [](int) { stop = true; });

    while(!stop){}
    running = false;

    th_send.join();
    th_rec.join();

    std::cout << "### FIN EXO2 ###" << std::endl;

    return 0;
}
