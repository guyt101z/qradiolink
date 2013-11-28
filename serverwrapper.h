#ifndef SERVERWRAPPER_H
#define SERVERWRAPPER_H

#include <QObject>
#include <QVector>
#include <QString>
#include <unistd.h>
#include "telnetserver.h"
#include "databaseapi.h"
#include "speech.h"

class ServerWrapper : public QObject
{
    Q_OBJECT
public:
    explicit ServerWrapper(DatabaseApi *db, QObject *parent = 0);
    void stop();

signals:
    void finished();
    void joinConference(QString number, QString ip, int id);
public slots:
    void run();
    void addSpeech(QString);
    void connectToConference(QString number, QString ip, int id);
private:
    bool _stop;
    Speech *_speech;
    QVector<QString> *_speech_text;
    DatabaseApi *_db;
};

#endif // SERVERWRAPPER_H
