#ifndef AUDIOINTERFACE_H
#define AUDIOINTERFACE_H

#include <QObject>
#include <stdio.h>
#include <pulse/simple.h>
#include <pulse/error.h>

class AudioInterface : public QObject
{
    Q_OBJECT
public:
    explicit AudioInterface(QObject *parent = 0, unsigned sample_rate = 8000, unsigned channels = 2);
    ~AudioInterface();
    int read(float *buf, short bufsize);
    int write(float *buf, short bufsize);
    
signals:
    
public slots:

private:
    pa_simple *_s_rec;
    pa_simple *_s_play;
    int _error;
    
};

#endif // AUDIOINTERFACE_H
