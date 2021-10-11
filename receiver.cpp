#include "receiver.hpp"

receiver::receiver(fifo &fif, int cols, int rows, float ratio, AVCodecID id, AVPixelFormat px, int fps) : fr(fif)
{
    codec.init(id, cols, rows, px, fps);
    width = cols;
    height = rows;
    this->ratio = ratio;
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
        codec.decode(out, pkt);
        if(!out.empty()){
            render(out);
        }
    }
    while(1)
    {
        ret = codec.finish_decode(out);
        if(ret < 0)
            return;
        render(out);
    }
}

void receiver::render(cv::Mat mat)
{
    cv::Mat res;
    cv::cvtColor(mat, mat, cv::COLOR_YUV2BGR);
    cv::resize(mat, res, cv::Size(), ratio, ratio);
    cv::imshow("receiver", res);
    cv::waitKey(1);
}

receiver::~receiver(){}; 