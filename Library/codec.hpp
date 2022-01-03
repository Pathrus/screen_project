#ifndef CODEC_HPP
#define CODEC_HPP

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavutil/opt.h>
    #include <libavutil/imgutils.h>
    #include <libavutil/frame.h>
    #include <libavutil/error.h>
    #include <libavformat/avformat.h>
}
#include "headers.h"

class mon_codec
{
    public:
        //mon_codec() = delete;

        //mon_codec();

        mon_codec(int w, int h, int fps);

        int init(AVCodecID id, int w, int h, AVPixelFormat px, int fps);

        void encode(cv::Mat mat, AVFrame *f, AVPacket *pkt_out);
        int finish_encode(AVPacket *pkt_out);

        void decode(cv::Mat mat, AVPacket *pkt, AVFrame *fout);
        int finish_decode(cv::Mat mat, AVFrame *fout);

        ~mon_codec();

    private:
        AVCodec *codec, *codec2;
        AVCodecContext *encoder, *decoder;
        AVFrame *frame;
        int nb;
};

#endif