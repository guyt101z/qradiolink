#ifndef AUDIOOP_H
#define AUDIOOP_H

#include <QObject>
#include <QCoreApplication>
#include <unistd.h>
#include <math.h>
#include "audiointerface.h"

class AudioOp : public QObject
{
    Q_OBJECT
public:
    explicit AudioOp(AudioInterface *audio, QObject *parent = 0);
    void stop();
signals:
    void finished();
    void audioData(short *data, short size);
public slots:
    void run();
    void pcmAudio(short *pcm, short samples);

private:
    AudioInterface *_audio;
    bool _stop;
    
};

#endif // AUDIOOP_H
