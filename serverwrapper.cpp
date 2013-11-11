#include "serverwrapper.h"
#include <cstdlib>

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
    qDebug() << "Server running";
    //server->run();
    while(true)
    {
        usleep(10000);
        QCoreApplication::processEvents();
        if(_stop)
            break;
    }
    server->stop();
    delete server;
    emit finished();
}
