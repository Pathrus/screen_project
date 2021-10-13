#include "headers.h"

#include "codec.hpp"
#include "ScreenCapture.hpp"

//#include "libyuv.h"
//#include "libyuv/convert.h"

bool running = true;

int main(int argc, char** argv)
{
    std::cout << "Debut screen_save - Ctrl + C pour stop - output file : save.h264" << std::endl;
    signal(SIGINT, [](int) { running = false; });
    uint8_t endcode[] = { 0, 0, 1, 0xb7 };
    int fps = 30;
    ScreenCapture cap;
    mon_codec c;
    c.init(AV_CODEC_ID_H264, cap.width, cap.height, AV_PIX_FMT_YUV420P, fps);

    FILE *out = fopen("save.h264", "wb");
    AVPacket *pkt = av_packet_alloc();
    AVFrame *f = av_frame_alloc();
    f->height = cap.height;
    f->width = cap.width;
    f->format = AV_PIX_FMT_YUV420P;
    av_frame_get_buffer(f, 0);
    cv::Mat yuv = cv::Mat(cap.height, cap.width, CV_8UC3);
    cv::Mat img;

    while(running)
    {
        img = cap.GetImage();

        //img in BGRA in memory, LSB so ARGB
        libyuv::ARGBToI420(img.data, img.step, f->data[0], cap.width, f->data[1], (cap.width+1)/2, f->data[2], (cap.width+1)/2, cap.width, cap.height);
        c.encode(yuv, f, pkt);
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
    std::cout << "### FIN SAVE ###" << std::endl;
    return 0;
}