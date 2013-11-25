#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
    _db = new DatabaseApi;
    _client = new AudioClient;
    _client->setProperties(QString("guest"),QString("guest"),QString("fgcom.flightgear.org"));
    //_client->init();
}


void Controller::haveCall(QVector<char> *dtmf)
{

    std::string number;
    for(int i=0;i<dtmf->size();i++)
    {
        if((dtmf->at(i)!='*') && (dtmf->at(i)!='C'))
        {
            number.push_back(dtmf->at(i));
            emit speak(QString(dtmf->at(i)));
        }
    }
    dtmf->clear();
    emit readyInput();
    Station *s  = _db->get_station_by_radio_id(QString::fromStdString(number));
    QVector<Server*> servers = _db->get_servers();
    if(servers.size() < 1)
    {
        QString voice= "There are no active servers in the list.";
        emit speak(voice);
        return;
    }
    if(s->_id != 0)
    {
        TelnetClient telnet;
        telnet.connectToHost(s->_ip,4939);
        telnet.send("join",QString::fromStdString(number));
        QString voice= "Calling the station into the conference.";
        emit speak(voice);
        _client->init();
        _client->makeCall(number);
    }
    else
    {
        QString voice= "I can't find the station with this number.";
        emit speak(voice);
    }

    delete s;
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
