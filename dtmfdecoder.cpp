#include "dtmfdecoder.h"
#define BUFSIZE 8

DtmfDecoder::DtmfDecoder(QObject *parent) :
    QObject(parent)
{
    _stop=false;

}

DtmfDecoder::~DtmfDecoder()
{

}

void DtmfDecoder::stop()
{
    _stop = true;
}

void DtmfDecoder::run()
{

    Dtmf *dtmf = new Dtmf;
    AudioInterface *audio= new AudioInterface(0,8000,2);
    while(true)
    {
        if(_stop)
            break;
        short int *buf =new short int[BUFSIZE];
        audio->read(buf,BUFSIZE);

        dtmf->decode(buf);

        delete[] buf;

    }
    delete dtmf;
    delete audio;
    emit finished();
}
