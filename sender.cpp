#include "sender.hpp"

sender::sender(fifo &fif, int rows, int cols, int fps) : fs(fif), width(rows), height(cols),\
                codec(sc.getWidth(), sc.getHeight(), fps), sock("127.0.0.1", 1234, "client")
{
    //codec.init(id, sc.getWidth(), sc.getHeight(), px, fps);
    //mon_codec codec(id, sc.getWidth(), sc.getHeight(), px, fps);
    f = av_frame_alloc();
    f->width = sc.getWidth();
    f->height = sc.getHeight();
    f->format = AV_PIX_FMT_YUV420P;
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
    int i = 0;
    int ret = 0;
    AVPacket *pkt = av_packet_alloc();
    AVPacket *pkt_send = av_packet_alloc();
    //AVPacket *pkt_sup = av_packet_alloc();  // to manage big packet
    cv::Mat yuv, img;
    /*std::string test ="test ";
    
    while(running)
    {
        std::string buffer = test + std::to_string(i);
        sock.sendTo(buffer.c_str(), buffer.length());
        i++;
        sleep(1);
    }
    */

    int size_max = 0xFF7F;
    while(running)
    {
        img = sc.GetImage();
        libyuv::ARGBToI420(img.data, img.step, f->data[0], f->width, f->data[1], (f->width+1)/2, f->data[2], (f->width+1)/2, f->width, f->height);

        codec.encode(yuv, f, pkt);
        if(pkt->size != 0)
        {
            //char* test = "test";
            av_packet_move_ref(pkt_send, pkt);

            //std::cout << "S-pts: " << pkt->pts << " dts: " << pkt->dts << " size: " << pkt->size \
            //<< " st_i: " << pkt->stream_index << " f" << pkt->flags << " dura " << pkt->duration << " pos" << pkt->pos << std::endl;


            std::cout << "Send pkt - size " << pkt_send->size << std::endl;
            if(pkt_send->size > size_max) // FFF7 car 8 bits de header + data
            {
                //
                
                int size = pkt_send->size;
                int nb = pkt_send->size / size_max;
                std::string msg = "pkt " + std::to_string(nb) + " " + std::to_string(pkt_send->size);
                //std::cout << msg << std::endl;
                //msg += std::to_string(nb);
                sock.sendTo(msg.c_str(), msg.length());
                
                for(auto i = 0; i < nb; i++)
                {
                    sock.sendTo(reinterpret_cast<char*>(pkt_send->data), size_max);
                }
                sock.sendTo(reinterpret_cast<char*>(pkt_send->data + size_max*nb), pkt_send->size - size_max*nb);
                //av_packet_from_data(pkt_sup, pkt)
            }
            else
                //sock.sendTo(test, sizeof(test));
                sock.sendTo(reinterpret_cast<char*>(pkt_send->data), pkt_send->size);
            //send(pkt);
        }
        i++;
    }
    while(1)
    {
        ret = codec.finish_encode(pkt);
        if(ret < 0)
            return;
        av_packet_move_ref(pkt_send, pkt);
        /*if(pkt->size > 0xFFFF)
        {
            char* msg = "2 pkt";
            sock.sendTo(reinterpret_cast<char*>(msg), sizeof(msg));
            sock.sendTo(reinterpret_cast<char*>(pkt_send->data), 0xFFFF);
            sock.sendTo(reinterpret_cast<char*>(pkt_send->data + 0xFFFF), pkt->size - 0xffff);
            //av_packet_from_data(pkt_sup, pkt)
        }
        else*/
            sock.sendTo(reinterpret_cast<char*>(pkt_send->data), pkt_send->size);

        //send(pkt);
        
        //sock.sendTo(reinterpret_cast<char*>(pkt_send->data), pkt_send->size);
    }
}

sender::~sender() {}
