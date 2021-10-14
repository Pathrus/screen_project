#ifndef SENDER_HPP
#define SENDER_HPP

#include "headers.h"
#include "fifo.hpp"
#include "ScreenCapture.hpp"
#include "codec.hpp"

class sender
{
    public:
        
        sender(fifo &fif, int rows, int cols, AVCodecID id, AVPixelFormat px, int fps);
        void send(AVPacket *pkt);
        void operator()();
        ~sender();

    private:
        fifo& fs;
        int nb, width, height;
        ScreenCapture sc;
        mon_codec codec;
        AVFrame* f;
};

#endif