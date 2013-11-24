#ifndef SERVERWRAPPER_H
#define SERVERWRAPPER_H

#include <QObject>
#include <QVector>
#include <QString>
#include <unistd.h>
#include "telnetserver.h"
#include "speech.h"

class ServerWrapper : public QObject
{
    Q_OBJECT
public:
    explicit ServerWrapper(QObject *parent = 0);
    void stop();

signals:
    void finished();
public slots:
    void run();
    void addSpeech(QString);
private:
    bool _stop;
    Speech *_speech;
    QVector<QString> *_speech_text;
};

#endif // SERVERWRAPPER_H
