#ifndef FIFO_HPP
#define FIFO_HPP

#include "stdio.h"
#include <iostream>
#include <mutex>
#include <queue>

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavutil/opt.h>
    #include <libavutil/imgutils.h>
    #include <libavutil/frame.h>
    #include <libavutil/error.h>
    #include <libavformat/avformat.h>
}

class fifo
{
    public:
        fifo();

        //fifo(const fifo& f);
        fifo(fifo &f);

        void push(AVPacket pkt);
        AVPacket pop();

        ~fifo();

    public:
        std::mutex mutex_fifo;
        int max, nb;
        std::queue<AVPacket> queue;
};

#endif