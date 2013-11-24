#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
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
    TelnetClient telnet;
    //telnet.connectToHost("localhost",4939);
    //telnet.send("join",QString::fromStdString(number));
    QString voice= "Calling the station into the conference.";

    emit speak(voice);
    _client->init();
    emit readyInput();
    dtmf->clear();
    _client->makeCall(number);


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
    QString voice= "Disconnecting from the conference.";
    emit speak(voice);
    emit readyInput();
    if(number=="9")
        _client->disconnectCall();


    dtmf->clear();
}
