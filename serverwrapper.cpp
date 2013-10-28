#include "serverwrapper.h"

ServerWrapper::ServerWrapper(QObject *parent) :
    QObject(parent)
{
    _stop=false;
}

void ServerWrapper::stop()
{
    _stop=true;
}

void ServerWrapper::run()
{
    TelnetServer *server = new TelnetServer;
    qDebug() << "wrapper running";
    server->run();
    while(true)
    {
        if(_stop)
            break;
    }
    server->stop();
    delete server;
    emit finished();
}
