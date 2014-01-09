#ifndef AUDIOOP_H
#define AUDIOOP_H

#include <QObject>
#include <QDebug>
#include <QCoreApplication>
#include <unistd.h>
#include <math.h>
#include "audiointerface.h"
#include "ext/agc.h"
#include "ext/vox.h"

class AudioOp : public QObject
{
    Q_OBJECT
public:
    explicit AudioOp(QObject *parent = 0);
    void stop();
signals:
    void finished();
    void audioData(short *data, short size);
public slots:
    void run();
    void pcmAudio(short *pcm, short samples);

private:
    bool _stop;
    AudioInterface *_audio;
};

#endif // AUDIOOP_H
