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

#include "serverwrapper.h"


ServerWrapper::ServerWrapper(Settings *settings, DatabaseApi *db, QObject *parent) :
    QObject(parent)
{
    _db = db;
    _stop=false;
    _speech_text = new QVector<QString>;
    _settings = settings;
}

void ServerWrapper::stop()
{
    _stop=true;
}


void ServerWrapper::addSpeech(QString s)
{
    _speech_text->append(s);
}

void ServerWrapper::run()
{
    TelnetServer *server = new TelnetServer(_db);
    QObject::connect(server,SIGNAL(joinConference(int, int,int)),this,SLOT(connectToConference(int, int, int)));
    QObject::connect(server,SIGNAL(leaveConference(int, int,int)),this,SLOT(disconnectFromConference(int, int, int)));
    qDebug() << "Server running";

    int last_time = 0;
    int last_ping_time = 0;

    Speech spp;
    while(true)
    {

        usleep(50000);
        int time = QDateTime::currentDateTime().toTime_t();
        if((time - last_time) > 300)
        {
            //spp.fspeak("This is Q radio link, test U H F.");
            last_time = time;
        }
        if(((time - last_ping_time) > 5) )
        {
            emit pingServer();
            last_ping_time = time;

        }


        for(int o =0;o<_speech_text->size();o++)
        {
            spp.fspeak(_speech_text->at(o).toLocal8Bit().data());
        }
        _speech_text->clear();
        QCoreApplication::processEvents();
        if(_stop)
            break;
    }
    server->stop();
    delete server;
    emit finished();
}


void ServerWrapper::connectToConference(int number, int id, int server_id)
{
    emit joinConference(number, id, server_id);
}

void ServerWrapper::disconnectFromConference(int number, int id, int server_id)
{
    emit leaveConference(number, id, server_id);
}
