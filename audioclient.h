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
signals:
    void readyInput();
    
public slots:
    void haveCall(QVector<char> *dtmf);
    void sendDTMF(char letter);

private:
    bool _initialized;
    bool _enabled;
    bool _register;
    QString _username;
    QString _password;
    QString _server;
    std::string computePhoneNumber(const double& freq, const std::string& icao) const;
    std::vector<const char*> get_available_devices();
    bool testForError(void *p, std::string s);
    bool testForALError(std::string s);
    bool testForALCError(std::string s);

};

#endif // AUDIOCLIENT_H
