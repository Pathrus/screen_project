#include "receiver.hpp"

receiver::receiver(fifo &fif, int cols, int rows, float ratio, int fps) : fr(fif),\
                codec(cols, rows, fps), sock("127.0.0.1", 1234, "server")
{
    //codec.init(id, cols, rows, px, fps);
    //codec(id, cols, rows, px, fps);
    width = cols;
    height = rows;
    this->ratio = ratio;
    f = av_frame_alloc();
    f->height = rows;
    f->height = cols;
    f->format = AV_PIX_FMT_YUV420P;
    av_frame_get_buffer(f, 0);
    
    sock.binding();
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
    
    /*char buffer[256];
    while(running)
    {
        sock.recvFrom(buffer, sizeof(buffer));
        std::cout << buffer << std::endl;
    }*/
    uint8_t padding[32] = "0x00000000";
    int size_max = 0xFF7F;
    int n;
    int nb = 0;
    char buffer[size_max];
    while (running)
    {
        //getElem(pkt);
        //nb = sock.recvFrom(reinterpret_cast<char*>(pkt->data), size_max);
        nb = sock.recvFrom(buffer, size_max);
        std::cout << "Receiv - size " << size_max <<", nb: " << nb << std::endl;
        // if(nb = 8) || nb == 1 ?
        if(strncmp(buffer, "pkt", 3) == 0)
        {
            n = atoi(buffer+4);
            int t = atoi(buffer + 6);
            char in[t + 32];
            std::cout << buffer << std::endl;
            //av_grow_packet(pkt, t); // AV_INPUT_BUFFER_PADDING_SIZE
            for(auto i = 0; i < n; i++)
            {
                //nb = sock.recvFrom(reinterpret_cast<char*>(pkt->data+i*size_max) , size_max);
                nb = sock.recvFrom(in + i*size_max, size_max);
                //nb = sock.recvFrom(buffer, size_max);
                //std::cout << i << " pkt size " << pkt->size << std::endl;
            }
            nb = sock.recvFrom(in + n*size_max, size_max);
            //nb = sock.recvFrom(reinterpret_cast<char*>(pkt->data+n*size_max) , size_max);
            //memcpy(reinterpret_cast<void*>(&pkt->data[t]), reinterpret_cast<void*>(padding), 32);
            //memcpy(reinterpret_cast<void*>(in), reinterpret_cast<void*>(padding), 32);
            av_packet_from_data(pkt, reinterpret_cast<uint8_t*>(in), t);
            //std::cout << "R-pts: " << pkt->pts << " dts: " << pkt->dts << " size: " << pkt->size \
            //    << " st_i: " << pkt->stream_index << " f" << pkt->flags << " pos" << pkt->pos << std::endl;
        }
        else{
            av_packet_from_data(pkt, reinterpret_cast<uint8_t*>(buffer), nb);
            //nb = sock.recvFrom(reinterpret_cast<char*>(pkt->data), pkt->size);
            //memcpy(reinterpret_cast<void*>(pkt->data), reinterpret_cast<void*>(buffer), nb);
            //std::cout << "Receiv - size " << pkt->size << std::endl;
        }
        //nb = sock.recvFrom(reinterpret_cast<char*>(pkt->data) + size_max*n, size_max);
        //nb = sock.recvFrom(buffer, size_max);

        std::cout << "decoder - size " << pkt->size << std::endl;

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
        //render(out, f);
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