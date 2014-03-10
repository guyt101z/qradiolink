#ifndef VIDEOENCODER_H
#define VIDEOENCODER_H

#include <QObject>
#include <QDebug>
#include <theora/theoraenc.h>
#include <theora/theoradec.h>
#include <ogg/ogg.h>

class VideoEncoder
{
public:
    VideoEncoder();
    ~VideoEncoder();
    void encode_theora(char *buf);

private:
    th_enc_ctx *_encoder;
};

#endif // VIDEOENCODER_H
