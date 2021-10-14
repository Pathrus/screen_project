#include "receiver.hpp"

receiver::receiver(fifo &fif, int cols, int rows, float ratio, AVCodecID id, AVPixelFormat px, int fps) : fr(fif)
{
    codec.init(id, cols, rows, px, fps);
    width = cols;
    height = rows;
    this->ratio = ratio;
    f = av_frame_alloc();
    f->height = rows;
    f->height = cols;
    f->format = px;
    av_frame_get_buffer(f, 0);
}

void receiver::getElem(AVPacket *pkt) { 
    AVPacket p = fr.pop();
    av_packet_move_ref(pkt, &p);
}

void receiver::operator()()
{
    int ret = 0;
    AVPacket *pkt = av_packet_alloc();
    cv::Mat out(height, width, CV_8UC3);
    
    while (running)
    {
        getElem(pkt);
        codec.decode(out, pkt, f);
        if(f->pts){
            render(out, f);
        }
    }
    while(1)
    {
        ret = codec.finish_decode(out, f);
        if(ret < 0)
            return;
        render(out, f);
    }
}

void receiver::render(cv::Mat mat, AVFrame *f)
{
    cv::Mat rgb(height, width, CV_8UC4);
    cv::Mat res;
    libyuv::I420ToARGB(f->data[0], f->width, f->data[1], (f->width+1)/2, f->data[2], (f->width+1)/2, rgb.data, f->width*4, width, height);
    av_frame_unref(f);
    cv::resize(rgb, res, cv::Size(), ratio, ratio);
    cv::imshow("receiver", res);
    cv::waitKey(1);
}

receiver::~receiver(){}; 