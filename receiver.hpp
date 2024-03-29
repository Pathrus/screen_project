#ifndef RECEIVER_HPP
#define RECEIVER_HPP


#include "headers.h"
#include "fifo.hpp"
#include "codec.hpp"

class receiver
{
    public:
        receiver(fifo &fif, int cols, int rows, float ratio, AVCodecID id, AVPixelFormat px, int fps);
        void getElem(AVPacket *pkt);
        void operator()();
        void render(cv::Mat mat);
        ~receiver();

    public:
        int width, height;
        mon_codec codec;
        fifo& fr;
        int max;
        float ratio;
};

#endif