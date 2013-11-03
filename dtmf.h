#ifndef DTMF_H
#define DTMF_H

#include <QObject>
#include <QDebug>
#include "DtmfDetector.hpp"
#include "DtmfGenerator.hpp"
#include <unistd.h>
#include <algorithm>
#include "audiointerface.h"

class Dtmf : public QObject
{
    Q_OBJECT
public:
    explicit Dtmf(QObject *parent = 0, unsigned frame_size = 160);
    ~Dtmf();
    short* transmitCode(short *buf);
    short *encode();
    void decode(short *buf);
    void decodeAudio(AudioInterface *audio);
    
signals:
    
public slots:

private:
    char _dialButtons[16];
    DtmfDetector *_dtmfDetector;
    
};

#endif // DTMF_H
