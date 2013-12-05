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

#include "controller.h"

Controller::Controller(DatabaseApi *db, QObject *parent) : QObject(parent)
{
    _db = db;
    _client = new AudioClient;
    _conference_stations = new QVector<Station*>;
    _dialing_number = "";
    _connectable = false;
    _telnet = NULL;
    _current_station = NULL;
    Station *s = _db->get_local_station();
    _id = s->_id;
    delete s;
    _telnet = new TelnetClient;
    //_client->init();
}

Controller::~Controller()
{
    _client->end();
    delete _telnet;
    delete _conference_stations;

    delete _client;
}

void Controller::haveCall(QVector<char> *dtmf)
{

    bool p2p = false;
    std::string number;
    for(int i=0;i<dtmf->size();i++)
    {
        if((dtmf->at(i)!='*') && (dtmf->at(i)!='C') && (dtmf->at(i)!='D'))
        {
            number.push_back(dtmf->at(i));
            emit speak(QString(dtmf->at(i)));
        }
    }
    if(dtmf->last()=='D')
    {
        p2p =true;
    }
    dtmf->clear();
    emit readyInput();
    _dialing_number = number;
    Station *s  = _db->get_station_by_radio_id(QString::fromStdString(number));

    if(s->_id != 0)
    {
        if(s->_active != 1)
        {
            QString voice= "The station you have tried to call is not active.";
            emit speak(voice);
            return;
        }
        if(!testConnection(s->_ip))
            return;
        getStationParameters(s);
        while(_current_station->_waiting==1)
        {
            QCoreApplication::processEvents();
        }
        if(p2p && (s->_in_call == 0))
        {
            QString voice= "Trying a direct call.";
            emit speak(voice);

            _client->setProperties("test","test",s->_ip);
            _client->init();
            _client->makeCall("777");
            _in_conference =1;
            _conference_id = "777";
            _conference_stations->append(s);
            return;
        }

        QVector<Server*> servers = _db->get_servers();
        if(servers.size() < 1)
        {
            QString voice= "There are no active servers in the list.";
            emit speak(voice);
            return;
        }
        Server *server = servers[0];
        if(s->_in_call == 1)
        {
            // check if the station is already in conference;
            // if it is, just join it
            // if we're calling a third party, notify our peers and join it's conference,
            // or notify the third party of our conference number
            // what to do if we have two peers in separate conferences

            QString voice= "Joining the station in the conference.";
            emit speak(voice);
            _client->setProperties(server->_username,server->_password,server->_ip);
            _client->init();
            _client->makeCall(s->_conference_id.toStdString());
            _in_conference =1;
            _conference_id = s->_conference_id;
            _conference_stations->append(s);
            Station *st = _db->get_local_station();
            st->_called_by=0;
            st->_conference_id=_conference_id;
            st->_in_call=1;
            _db->update_station_parameters(st);
            delete st;
        }
        else
        {
            // if it's not, get the number of the first free conference and make a new conference
            _conference_id = getFreeConference();

            _telnet->send("JOIN",_conference_id+";"+QString::number(_id)+";"+QString::number(server->_id));
            QString voice= "Calling the station into the conference.";
            emit speak(voice);
            QObject::connect(_client,SIGNAL(callEnded()),this,SLOT(disconnectedFromCall()));
            _client->setProperties(server->_username,server->_password,server->_ip);
            _client->init();
            _client->makeCall(_conference_id.toStdString());
            _in_conference =1;
            _conference_stations->append(s);
            Station *st = _db->get_local_station();
            st->_called_by=0;
            st->_conference_id=_conference_id;
            st->_in_call=1;
            _db->update_station_parameters(st);
            delete st;
        }

    }
    else
    {
        QString voice= "I can't find the station with this number.";
        emit speak(voice);
        delete s;
    }
}


void Controller::haveCommand(QVector<char> *dtmf)
{
    std::string number;
    for(int i=0;i<dtmf->size();i++)
    {
        if((dtmf->at(i)!='*') && (dtmf->at(i)!='#'))
        {
            number.push_back(dtmf->at(i));
        }
    }
    dtmf->clear();
    emit readyInput();

    if(number=="9")
    {
        QString voice= "Disconnecting from the conference.";
        emit speak(voice);
        disconnectedFromCall();
        _client->disconnectCall();

    }
    if(number=="99")
    {
        QString voice= "Disconnecting from the conference.";
        emit speak(voice);
        _client->disconnectCall();
    }

}

bool Controller::testConnection(QString host)
{
    _telnet->disconnectHost();
    QObject::connect(_telnet,SIGNAL(connectedToHost()),this,SLOT(readyConnect()));
    QObject::connect(_telnet,SIGNAL(connectionFailure()),this,SLOT(noConnection()));
    QObject::connect(_telnet,SIGNAL(disconnectedFromHost()),this,SLOT(disconnectedLink()));
    _telnet->connectHost(host,CONTROL_PORT);
    int time = QDateTime::currentDateTime().toTime_t();
    while ((QDateTime::currentDateTime().toTime_t() - time) < 5)
    {
        QCoreApplication::processEvents();
        if(_connectable)
        {

            _connectable = false;
            return true;
        }
    }

    QString voice= "I cannot connect to host.";
    emit speak(voice);
    _connectable = false;


    return false;
}

void Controller::noConnection()
{
    QString voice= "Network error.";
    emit speak(voice);
    _connectable = false;
    delete _telnet;
    _telnet = NULL;
}

void Controller::readyConnect()
{
    _connectable = true;
}

void Controller::getStationParameters(Station *s)
{
    _current_station = s;
    _current_station->_waiting=1;

    QObject::connect(_telnet,SIGNAL(haveMessage(QString)),this,SLOT(setStationParameters(QString)));
    _telnet->send("PARAMETERS",QString::number(s->_id));

}

void Controller::setStationParameters(QString param)
{
    QStringList pre = param.split(";");
    if(pre.size()<2)
    {
        qDebug() << "invalid message";
    }
    _current_station->_in_call=pre[1].toInt();
    _current_station->_conference_id=pre[2];
    _current_station->_called_by=pre[3].toInt();
    _current_station->_waiting=0;
    _db->update_station_parameters(_current_station);
    QObject::disconnect(_telnet,SIGNAL(haveMessage(QString)),this,SLOT(setStationParameters(QString)));
}

QString Controller::getFreeConference()
{
    return "777"; //TODO:
}

void Controller::joinConference(QString number, int id, int server_id)
{
    /*
    Server *server = _db->get_server_by_id(server_id);
    if(server->_id < 1)
    {
        QString voice= "There are no active servers in the list.";
        emit speak(voice);
        return;
    }
    */
    QString voice= "Joining conference.";
    emit speak(voice);
    Station *s = _db->get_local_station();
    s->_called_by=id;
    s->_conference_id=number;
    s->_in_call=1;
    _db->update_station_parameters(s);
    delete s;
    s = _db->get_station_by_id(id);
    QString caller = s->_callsign;
    delete s;
    voice = "Called by " + caller;
    emit speak(voice);
    QObject::connect(_client,SIGNAL(callEnded()),this,SLOT(disconnectedFromCall()));
    //_client->setProperties(server->_username,server->_password,server->_ip);
    _client->setProperties("adrian","supersecret","192.168.1.2");
    _client->init();
    _client->makeCall(number.toStdString());
    _in_conference =1;

    //_conference_stations->append(_current_station);
}

void Controller::disconnectedFromCall()
{
    QString voice= "Conference has ended.";
    emit speak(voice);
    QObject::disconnect(_client,SIGNAL(callEnded()),this,SLOT(disconnectedFromCall()));
    if(_in_conference==1)
    {
        for(int i =0;i<_conference_stations->size();i++)
        {
            Station *s=_conference_stations->at(i);
            if(s->_called_by==_id)
            {
                //FIXME:
                _telnet->disconnectHost();
                _telnet->connectHost(s->_ip,CONTROL_PORT);
                _telnet->send("LEAVE","1");
            }
            s->_in_call=0;
            s->_called_by=0;
            s->_conference_id="";
            _db->update_station_parameters(s);
            delete s;
        }
        _conference_stations->clear();
    }
    _in_conference=0;
    Station *s = _db->get_local_station();
    s->_called_by=0;
    s->_conference_id="";
    s->_in_call=0;
    _db->update_station_parameters(s);
    delete s;
}

void Controller::disconnectedLink()
{

}

void Controller::leaveConference(QString number, int id, int server_id)
{
    disconnectedFromCall();
    _client->disconnectCall();
}
