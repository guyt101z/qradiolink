// Written by Adrian Musceac YO8RZZ at gmail dot com, started October 2013.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef AUDIOCLIENT_H
#define AUDIOCLIENT_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QVector>
#include "speech.h"
#include "config_defines.h"
#include "iaxclient/lib/iaxclient.h"


class IaxClient : public QObject
{
    Q_OBJECT
public:
    explicit IaxClient(QObject *parent = 0);
    ~IaxClient();
    void init();
    void iaxTextEvent(struct iaxc_ev_text text);
    void iaxCallEvent(struct iaxc_ev_call_state state);
    void setProperties(QString username, QString password, QString server);
    void makeCall(std::string number);
    void sendDTMF(char letter);
    void disconnectCall();
    void end();
signals:
    void callAnswered();
    void callEnded();

    
public slots:



private:
    bool _initialized;
    bool _enabled;
    bool _register;
    int _call;
    QString _username;
    QString _password;
    QString _server;
    std::vector<const char*> get_available_devices();


};

#endif // AUDIOCLIENT_H
