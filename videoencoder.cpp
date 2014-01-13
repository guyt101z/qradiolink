#include "videoencoder.h"

VideoEncoder::VideoEncoder()
{
    th_info param;
    param.frame_height = 480;
    param.frame_width = 640;
    param.pic_height = 320;
    param.pic_width = 240;
    param.colorspace = TH_CS_ITU_REC_470BG;
    param.pixel_fmt = TH_PF_444;
    param.target_bitrate = 50000;
    param.fps_denominator = 1;
    param.fps_numerator = 10;
    _encoder = th_encode_alloc(&param);
}

VideoEncoder::~VideoEncoder()
{
    th_encode_free(_encoder);
}


void VideoEncoder::encode_theora(char *buf)
{
    ogg_packet *packet = new ogg_packet;
    th_comment comment;
    while(th_encode_flushheader(_encoder, &comment, packet) > 0)
    {
        qDebug() << packet->bytes;
    }
    //th_encode_ycbcr_in(_encoder, buf);
    while(th_encode_packetout(_encoder,1,packet) > 0)
    {

    }
}
