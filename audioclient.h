#ifndef AUDIOCLIENT_H
#define AUDIOCLIENT_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QVector>
#include "speech.h"
#include "iaxclient/lib/iaxclient.h"


class AudioClient : public QObject
{
    Q_OBJECT
public:
    explicit AudioClient(QObject *parent = 0);
    ~AudioClient();
    void init();
    void iaxTextEvent(struct iaxc_ev_text text);
    void iaxCallEvent(struct iaxc_ev_call_state state);
    void setProperties(QString username, QString password, QString server);
    void makeCall(std::string number);
    void sendDTMF(char letter);
    void disconnectCall();
signals:
    void readyInput();
    
public slots:



private:
    bool _initialized;
    bool _enabled;
    bool _register;
    QString _username;
    QString _password;
    QString _server;
    std::string computePhoneNumber(const double& freq, const std::string& icao) const;
    std::vector<const char*> get_available_devices();


};

#endif // AUDIOCLIENT_H
