#include "telnetserver.h"

TelnetServer::TelnetServer(QObject *parent) :
    QObject(parent)
{
    _hostname = QHostAddress::Any;
    _listen_port = 5030;
    _stop=false;
    _server = new QTcpServer;
    _server->listen(_hostname,_listen_port);
    QObject::connect(_server,SIGNAL(newConnection()),this,SLOT(getConnection()));
    qDebug() << "Telnet server init completed";
}

TelnetServer::~TelnetServer()
{
    _server->close();
    delete _server;
    qDebug() << "Telnet server exiting";
}

void TelnetServer::stop()
{
    _stop = true;
    for(int i =0;i<_connected_clients.size();i++)
    {
        QTcpSocket *s = _connected_clients.at(i);
        s->write("Server is stopping now.");
        s->disconnectFromHost();
    }
    _connected_clients.clear();
}

void TelnetServer::getConnection()
{
    // ok
    QTcpSocket *socket = _server->nextPendingConnection();
    QObject::connect(socket,SIGNAL(error(QAbstractSocket::SocketError )),this,SLOT(connectionFailed(QAbstractSocket::SocketError)));
    QObject::connect(socket,SIGNAL(connected()),this,SLOT(connectionSuccess()));
    QObject::connect(socket,SIGNAL(readyRead()),this,SLOT(processData()));
    QObject::connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    _connected_clients.append(socket);

}

void TelnetServer::connectionFailed(QAbstractSocket::SocketError error)
{
    // ok
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(QObject::sender());
    qDebug() << "Connection error: " << socket->errorString();
    int i = _connected_clients.indexOf(socket);
    _connected_clients.remove(i);

}

void TelnetServer::connectionSuccess()
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(QObject::sender());
    qDebug() << "Connect";
    qDebug() << "Connection incoming: " << socket->peerAddress().toString() << ":" << socket->peerPort();
}


void TelnetServer::run()
{
    /*

    while(true)
    {
        if(_stop)
            break;
        QCoreApplication::processEvents();
        for(int i=0;i<_connected_clients.size();i++)
        {
            QTcpSocket *socket = _connected_clients.at(i);


        }
    }

    */
}

void TelnetServer::processData()
{
    //ok
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(QObject::sender());
    qDebug() << "Received message from: " << socket->peerAddress().toString();

    QString line;

    bool endOfLine = false;

    while ((!endOfLine))
    {

        char ch;
        if(socket->bytesAvailable()>0)
        {
            int bytesRead = socket->read(&ch, sizeof(ch));
            if (bytesRead == sizeof(ch))
            {
                //cnt++;

                if ((ch == '\r'))
                {
                    endOfLine = true;


                }
                else
                {
                    line.append( ch );
                }
            }
        }
        else
        {
            continue;
        }


    }
    socket->write(line.toUtf8());


    //emit haveProperty(line);
}
