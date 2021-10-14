#include "sender.hpp"

sender::sender(fifo &fif, int rows, int cols, AVCodecID id, AVPixelFormat px, int fps) : fs(fif), width(rows), height(cols)
{
    codec.init(id, sc.getWidth(), sc.getHeight(), px, fps);
    f = av_frame_alloc();
    f->width = sc.getWidth();
    f->height = sc.getHeight();
    f->format = px;
    av_frame_get_buffer(f, 0);
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
        libyuv::ARGBToI420(img.data, img.step, f->data[0], f->width, f->data[1], (f->width+1)/2, f->data[2], (f->width+1)/2, f->width, f->height);

        codec.encode(yuv, f, pkt);
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
