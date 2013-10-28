#ifndef SPEECH_H
#define SPEECH_H

#include <QObject>
#include "audiointerface.h"
#include <espeak/speak_lib.h>

class Speech : public QObject
{
    Q_OBJECT
public:
    explicit Speech(QObject *parent = 0);
    void speak(char* text);
    
signals:
    
public slots:

private:

    
};
int SynthCallback(short *wav, int numsamples, espeak_EVENT *events);

#endif // SPEECH_H
