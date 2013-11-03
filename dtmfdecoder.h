#ifndef DTMFDECODER_H
#define DTMFDECODER_H

#define INT16_MAX 0x7FFF
#define INT16_MIN (-INT16_MAX - 1)
#define INT32_MAX (2147483647)
#define INT32_MIN (-2147483647 - 1)
#define UINT16_MAX 65535
#define UINT8_MAX 255


#include <QObject>
#include <QtDebug>
#include "audiointerface.h"
#include <spandsp.h>
#include "dtmf.h"

class DtmfDecoder : public QObject
{
    Q_OBJECT
public:
    explicit DtmfDecoder(QObject *parent = 0);
    ~DtmfDecoder();
    void stop();
    
signals:
    void finished();
    
public slots:
    void run();

private:
    bool _stop;
    float *makeTone(int samplerate, float frequency, int length, float gain=1.0);
    float goertzel(float *x, int N, float frequency, int samplerate);
    float power(float value);
    float _dtmf_frequencies[7];
    
};

#endif // DTMFDECODER_H
