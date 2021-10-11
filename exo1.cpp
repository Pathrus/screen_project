#include "headers.h"

#include "codec.hpp"
#include "ScreenCapture.hpp"

bool running = true;

int main(int argc, char** argv)
{
    std::cout << "Debut exo1 - Ctrl + C pour stop - output file : exo1.h264" << std::endl;
    signal(SIGINT, [](int) { running = false; });
    uint8_t endcode[] = { 0, 0, 1, 0xb7 };
    int fps = 30;
    ScreenCapture cap;
    mon_codec c;
    c.init(AV_CODEC_ID_H264, cap.width, cap.height, AV_PIX_FMT_YUV444P, fps);
    FILE *out = fopen("exo1.h264", "wb");
    AVPacket *pkt = av_packet_alloc();
    cv::Mat yuv, img;

    while(running)
    {
        img = cap.GetImage();
        cv::cvtColor(img, yuv, cv::COLOR_BGR2YUV);
        c.encode(yuv, pkt);
        fwrite(pkt->data, 1, pkt->size, out);
    }
    
    int ret = 0;
    while(ret >= 0)
    {
        ret = c.finish_encode(pkt);
        if(ret < 0)
            break;
        fwrite(pkt->data, 1, pkt->size, out);
    }
    fwrite(endcode, 1, sizeof(endcode), out);
    fclose(out);
    std::cout << "### FIN EXO1 ###" << std::endl;
    return 0;
}