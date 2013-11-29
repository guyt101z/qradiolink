#include "serverwrapper.h"
#include <cstdlib>

ServerWrapper::ServerWrapper(DatabaseApi *db, QObject *parent) :
    QObject(parent)
{
    _db = db;
    _stop=false;
    _speech_text = new QVector<QString>;
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
    QObject::connect(server,SIGNAL(joinConference(QString,QString, int)),this,SLOT(connectToConference(QString, QString, int)));
    qDebug() << "Server running";
    //server->run();
    int last_time = 0;
    Speech spp;
    while(true)
    {
        usleep(10000);
        int time = QDateTime::currentDateTime().toTime_t();
        if((time - last_time) > 60)
        {
            spp.fspeak("This is Q radio link, test U H F.");
        }
        for(int o =0;o<_speech_text->size();o++)
        {
            spp.fspeak(_speech_text->at(o).toLocal8Bit().data());
        }
        _speech_text->clear();
        last_time = time;
        QCoreApplication::processEvents();
        if(_stop)
            break;
    }
    server->stop();
    delete server;
    emit finished();
}

void ServerWrapper::connectToConference(QString ip, QString number, int id)
{
    emit joinConference(ip,number, id);
}
