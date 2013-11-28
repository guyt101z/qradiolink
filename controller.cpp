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
    //TODO: fetch from database
    _username="adrian";
    _password= "supersecret";
    Station *s = _db->get_local_station();
    _id = s->_id;
    _telnet = new TelnetClient;
    _client->init();
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

        }
        if(p2p && (s->_in_call == 0))
        {
            QString voice= "Trying a direct call.";
            emit speak(voice);

            _client->setProperties(_username,_password,s->_ip);
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
            _client->setProperties(_username,_password,server->_ip);
            _client->makeCall(number.toStdString());
            _in_conference =1;
            _conference_id = number;
            _conference_stations->append(s);
        }
        else
        {
            // if it's not, get the number of the first free conference and make a new conference
            _conference_id = getFreeConference();

            _telnet->send("join",server->_ip.append(";").append(_conference_id).append(";").append(_id));
            QString voice= "Calling the station into the conference.";
            emit speak(voice);
            _client->setProperties(_username,_password,server->_ip);
            _client->makeCall(number);
            _in_conference =1;
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
    if(number=="99")
    {
        QString voice= "Disconnecting from the conference.";
        emit speak(voice);
        _client->disconnectCall();
    }
    dtmf->clear();
    emit readyInput();

}

bool Controller::testConnection(QString host)
{

    QObject::connect(_telnet,SIGNAL(connectedToHost()),this,SLOT(readyConnect()));
    QObject::connect(_telnet,SIGNAL(connectionFailure()),this,SLOT(noConnection()));

    _telnet->connectHost(host,4939);
    int time = QDateTime::currentDateTime().toTime_t();
    while ((QDateTime::currentDateTime().toTime_t() - time) < 6)
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

    QObject::connect(_telnet,SIGNAL(haveProperty(QString)),this,SLOT(setStationParameters(QString)));
    _telnet->send("parameters",QString::number(s->_id));

}

void Controller::setStationParameters(QString param)
{
    QStringList pre = param.split(";");
    _current_station->_in_call=pre[0].toInt();
    _current_station->_conference_id=pre[1];
    _current_station->_called_by=pre[2].toInt();
    _current_station->_waiting=0;
}

QString Controller::getFreeConference()
{
    return "777";
}

void Controller::joinConference(QString number, QString ip, int id)
{

    QString voice= "Joining conference.";
    emit speak(voice);
    _client->setProperties(_username,_password,ip);
    _client->makeCall(number.toStdString());
    _in_conference =1;

    //_conference_stations->append(_current_station);
}
