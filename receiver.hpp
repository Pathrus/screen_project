#ifndef RECEIVER_HPP
#define RECEIVER_HPP


#include "headers.h"
#include "fifo.hpp"
#include "codec.hpp"
#include "socket.hpp"

class receiver
{
    public:
        receiver(fifo &fif, int cols, int rows, float ratio, int fps);
        void getElem(AVPacket *pkt);
        void operator()();
        void render(cv::Mat mat, AVFrame *f);
        ~receiver();

    private:
        AVFrame *f;
        int width, height;
        mon_codec codec;
        fifo& fr;
        int max;
        float ratio;
        Socket sock;
};

#endif