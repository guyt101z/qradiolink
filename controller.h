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
    void readyConnect();
    void noConnection();
    void setStationParameters(QString param);
    void joinConference(QString number, QString ip);
public:
    Controller(DatabaseApi *db, QObject *parent = 0);

signals:
    void readyInput();
    void speak(QString);
private:
    AudioClient *_client;
    DatabaseApi *_db;
    int _in_conference;
    QString _conference_id;
    QVector<Station*> *_conference_stations;
    std::string _dialing_number;
    bool _connectable;
    bool testConnection(QString host);
    void getStationParameters(Station *s);
    QString getFreeConference();
    Station *_current_station;
    QString _username;
    QString _password;
};

#endif // CONTROLLER_H
