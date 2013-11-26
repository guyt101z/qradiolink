#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
    _db = new DatabaseApi;
    _client = new AudioClient;
    _conference_stations = new QVector<Station*>;
    _dialing_number = "";
    _client->init();
}


void Controller::haveCall(QVector<char> *dtmf)
{
    QString username = "test";
    QString password = "test";
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
        if(p2p && (s->_in_call == 0))
        {
            QString voice= "Trying a direct call.";
            emit speak(voice);
            TelnetClient t;
            QObject::connect(&t,SIGNAL(connectedToHost()),this,SLOT(readyConnect()));
            QObject::connect(&t,SIGNAL(connectionFailure()),this,SLOT(noConnection()));
            t.connectToHost(s->_ip,4939);
            _client->setProperties(username,password,s->_ip);
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
            QString number = s->_conference_id;
            QString voice= "Joining the station in the conference.";
            emit speak(voice);
            _client->setProperties(username,password,server->_ip);
            _client->makeCall(number.toStdString());
            _in_conference =1;
            _conference_id = number;
            _conference_stations->append(s);
        }
        else
        {
            // if it's not, get the number of the first free conference and make a new conference
            TelnetClient telnet;
            telnet.connectToHost(s->_ip,4939);
            telnet.send("join",QString::fromStdString(number));
            QString voice= "Calling the station into the conference.";
            emit speak(voice);
            _client->setProperties(username,password,server->_ip);
            _client->makeCall(number);
            _in_conference =1;
            _conference_id = "number"; // TODO: FIXME:
            _conference_stations->append(s);
        }

    }
    else
    {
        QString voice= "I can't find the station with this number.";
        emit speak(voice);
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


    if(number=="9")
    {
        QString voice= "Disconnecting from the conference.";
        emit speak(voice);
        _client->disconnectCall();
    }
    dtmf->clear();
    emit readyInput();

}

void Controller::noConnection()
{
    QString voice= "I cannot connect to host.";
    emit speak(voice);
}

void Controller::readyConnect()
{

}
