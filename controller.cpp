#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
    _client = new AudioClient;
    _client->setProperties(QString("guest"),QString("guest"),QString("fgcom.flightgear.org"));
    //_client->init();
}


void Controller::haveCall(QVector<char> *dtmf)
{
    Speech spp;
    std::string number;
    for(int i=0;i<dtmf->size();i++)
    {
        if((dtmf->at(i)!='*') && (dtmf->at(i)!='C'))
        {
            number.push_back(dtmf->at(i));
            spp.fspeak(QString(dtmf->at(i)).toLocal8Bit().data());
        }
    }
    TelnetClient telnet;
    //telnet.connectToHost("localhost",4939);
    //telnet.send("join",QString::fromStdString(number));
    QString voice= "Calling the station into the conference.";
    qDebug() << QString::fromStdString(number);
    spp.fspeak(voice.toLocal8Bit().data());
    _client->init();
    emit readyInput();
    _client->makeCall(number);


    dtmf->clear();
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

    emit readyInput();
    if(number=="9")
        _client->disconnectCall();


    dtmf->clear();
}
