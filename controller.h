#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "audioclient.h"
#include "speech.h"
#include "telnetclient.h"

class Controller : public QObject
{
    Q_OBJECT
public slots:
    void haveCall(QVector<char> *dtmf);
    void haveCommand(QVector<char> *dtmf);
public:
    Controller(QObject *parent = 0);
signals:
    void readyInput();
private:
    AudioClient *_client;
};

#endif // CONTROLLER_H
