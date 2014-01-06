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
#include <cstdlib>

ServerWrapper::ServerWrapper(AudioInterface *audio, DatabaseApi *db, QObject *parent) :
    QObject(parent)
{
    _db = db;
    _stop=false;
    _speech_text = new QVector<QString>;
    _audio = audio;
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
    int audiobuffer_size = 640; //40 ms
    double treshhold = -15;
    double hyst = 0.2;
    bool treshhold_set = false;
    bool hyst_active = false;
    int hyst_counter = 0;
    Speech spp;
    while(true)
    {
        usleep(10000);
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
        short audiobuffer[audiobuffer_size];
        _audio->read_short(audiobuffer,audiobuffer_size);
        float sum=1.0;
        for(int j=0;j< audiobuffer_size;j++)
        {
            sum += static_cast<float>(audiobuffer[j]*audiobuffer[j]);
        }

        float rms = sqrt(sum/(static_cast<float>(audiobuffer_size)));
        double power = 20*log10(rms/32768.0f);
        if(!treshhold_set)
        {
            treshhold = power;
            treshhold_set = true;
        }

        if(power > treshhold+hyst)
        {
            if(!hyst_active)
            {
                hyst -= 5.0;
                hyst_active = true;
            }
            else
            {
                hyst_counter++;
            }
            emit audioData(audiobuffer,audiobuffer_size);
        }
        if((hyst_active) && (hyst_counter>50))
        {
            hyst +=5.0;
            hyst_active = false;
            hyst_counter = 0;
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

void ServerWrapper::pcmAudio(short *pcm, short samples)
{
    _audio->write_short(pcm,samples*sizeof(short));
    delete[] pcm;
}

void ServerWrapper::connectToConference(int number, int id, int server_id)
{
    emit joinConference(number, id, server_id);
}

void ServerWrapper::disconnectFromConference(int number, int id, int server_id)
{
    emit leaveConference(number, id, server_id);
}
