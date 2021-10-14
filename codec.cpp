#include "codec.hpp"

mon_codec::mon_codec() {}

int mon_codec::init(AVCodecID id, int w, int h, AVPixelFormat px, int fps)
{
    avcodec_register_all();
    codec = avcodec_find_encoder(id);
    encoder = avcodec_alloc_context3(codec);
    encoder->width = w;
    encoder->height = h;
    encoder->pix_fmt = px;
    encoder->time_base = (AVRational) {1,fps};
    encoder->framerate = (AVRational) {fps,1};
    encoder->gop_size = fps/2;
    encoder->max_b_frames = 1;
    encoder->bit_rate = w * h * fps * 0.5;

    av_opt_set(encoder->priv_data, "preset", "veryfast", 0);
    av_opt_set(encoder->priv_data, "tune", "zerolatency", 0);
    if(avcodec_open2(encoder, codec, NULL) < 0)
    {
        fprintf(stderr, "could not open codec\n");
        exit(1);
    }

    codec2 = avcodec_find_decoder(id);

    decoder = avcodec_alloc_context3(codec2);
    if(avcodec_open2(decoder, codec2, NULL) < 0)
    {
        fprintf(stderr, "could not open codec\n");
        exit(1);
    }

    frame = av_frame_alloc();    
    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }
    frame->height = h;
    frame->width = w;
    frame->format = px;
    av_frame_get_buffer(frame, 0);
    nb = 0;
}

void mon_codec::encode(cv::Mat mat, AVFrame* f, AVPacket *pkt_out)
{
    AVPacket pkt;
    int ret, got;
/*    cv::Mat spl[3];
    cv::split(mat, spl);
    frame->data[0] = spl[0].data;
    frame->data[1] = spl[1].data;
    frame->data[2] = spl[2].data;
*/    
    frame = av_frame_clone(f);

    av_init_packet(&pkt);
    pkt.data = NULL;
    pkt.size = 0;

    frame->pts = ++nb;
    ret = avcodec_encode_video2(encoder, &pkt, frame, &got);

    if(ret < 0) {
        fprintf(stderr, "Error encoding frame\n");
        exit(1);
    }
    if (got) {
        av_packet_move_ref(pkt_out, &pkt);
        av_packet_unref(&pkt);
    }                
}

int mon_codec::finish_encode(AVPacket *pkt_out)
{
    int ret = 1, got;
    AVPacket pkt;
    av_init_packet(&pkt);
    pkt.data = NULL;
    pkt.size = 0;

    ret = avcodec_send_frame(encoder, NULL);
            
    ret = avcodec_receive_packet(encoder, &pkt);
    if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        return ret;
        
    else if (ret < 0) {
        fprintf(stderr, "Error encoding frame\n");
        exit(1);
    }
            
    av_packet_move_ref(pkt_out, &pkt);            
    av_packet_unref(&pkt);
    return 0;
}

void mon_codec::decode(cv::Mat mat, AVPacket *pkt, AVFrame *fout)
{
    AVPacket d;
    av_init_packet(&d);
    av_packet_move_ref(&d, pkt);
    AVFrame *f = av_frame_alloc();
    cv::Mat c[3];
    int ret, got;
    ret = avcodec_send_packet(decoder, &d);
    while(ret >= 0)
    { 
        ret = avcodec_receive_frame(decoder, f);
        if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        if(ret < 0){
            fprintf(stderr, "Error decoding frame\n");
            exit(1);
        }
/*
        cv::split(mat, c);
        c[0].data = f->data[0];
        c[1].data = f->data[1];
        c[2].data = f->data[2];
        cv::merge(c, 3, mat);
*/
        //av_frame_copy(fout, f);
        av_frame_move_ref(fout, f);
        //printf("pts3 %d\n", f->pts);
    }
}

int mon_codec::finish_decode(cv::Mat mat, AVFrame *fout)
{
    AVFrame *f;
    f = av_frame_alloc();
    int ret = 1, got;
    cv::Mat c[3];
    ret = avcodec_send_packet(decoder, NULL);
    ret = avcodec_receive_frame(decoder, f);

    if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        return ret;
    
    else if (ret < 0) {
        fprintf(stderr, "Error encoding frame\n");
        exit(1);
    }
/*
    cv::split(mat, c);
    c[0].data = f->data[0];
    c[1].data = f->data[1];
    c[2].data = f->data[2];
    cv::merge(c, 3, mat);
*/    //av_frame_copy(fout, f);
    av_frame_move_ref(fout, f);
    return 0;
}

mon_codec::~mon_codec()
{
    av_frame_free(&frame);
}
