#ifndef SPEECH_H
#define SPEECH_H

#include <QObject>
#include "audiointerface.h"
#include <espeak/speak_lib.h>
#include <festival/festival.h>

class Speech : public QObject
{
    Q_OBJECT
public:
    explicit Speech(QObject *parent = 0);
    void speak(char* text);
    void fspeak(char* text);
    void speakChar(char c);
    AudioInterface *_audio;
    
signals:
    
public slots:

private:

    
};
int SynthCallback(short *wav, int numsamples, espeak_EVENT *events);

#endif // SPEECH_H
