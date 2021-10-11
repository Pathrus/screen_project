#include "sender.hpp"

sender::sender(fifo &fif, int rows, int cols, AVCodecID id, AVPixelFormat px, int fps) : fs(fif), width(rows), height(cols)
{
    codec.init(id, rows, cols, px, fps);
}

void sender::send(AVPacket *pkt)
{
    AVPacket p;
    av_packet_move_ref(&p, pkt);
    fs.push(p);
}

void sender::operator()()
{
    int ret = 0;
    AVPacket *pkt = av_packet_alloc();
    cv::Mat yuv, img;
    while(running)
    {
        img = sc.GetImage();
        cv::cvtColor(img, yuv, cv::COLOR_BGR2YUV);
        codec.encode(yuv, pkt);
        if(pkt->size != 0)
        {
            send(pkt);
        }
    }
    while(1)
    {
        ret = codec.finish_encode(pkt);
        if(ret < 0)
            return;
        send(pkt);
    }
}

sender::~sender() {}
