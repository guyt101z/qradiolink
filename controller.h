#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QString>
#include "audioclient.h"
#include "speech.h"
#include "telnetclient.h"
#include "databaseapi.h"
#include "station.h"

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
    void speak(QString);
private:
    AudioClient *_client;
    DatabaseApi *_db;
};

#endif // CONTROLLER_H
