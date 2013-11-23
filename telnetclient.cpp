#include "telnetclient.h"


static QString CRLF ="\r\n";

TelnetClient::TelnetClient(QObject *parent) :
    QObject(parent)
{
    _socket = new QTcpSocket;
    QObject::connect(_socket,SIGNAL(error(QAbstractSocket::SocketError )),this,SLOT(connectionFailed(QAbstractSocket::SocketError)));
    QObject::connect(_socket,SIGNAL(connected()),this,SLOT(connectionSuccess()));
    QObject::connect(_socket,SIGNAL(readyRead()),this,SLOT(processData()));
    _connection_tries=0;
    _status=0;
    _hostname = "localhost";
    _port= 5030;
}


TelnetClient::~TelnetClient()
{
    _socket->disconnectFromHost();
    delete _socket;
}


void TelnetClient::connectionSuccess()
{

    this->dataMode();
    _status=1;
    _connection_tries=0;
    emit connectedToHost();
}


void TelnetClient::connectionFailed(QAbstractSocket::SocketError error)
{
    _status=0;

    _connection_tries++;
    if(_connection_tries < 10)
    {
        this->connectToHost(_hostname,_port);
    }
    else
    {

        emit connectionFailure();
    }

}


void TelnetClient::connectToHost(const QString &host, const unsigned &port)
{
    if(_status==1) return;
    _socket->connectToHost(host, port);
    _hostname = host;
    _port = port;

}

void TelnetClient::disconnectFromHost()
{
    _socket->disconnectFromHost();
    _status=0;
    _connection_tries=0;
    emit connectionFailure();
}


void TelnetClient::send(QString prop_name, QString value)
{
    QString command = "set " + prop_name + " " + value + CRLF;
    _socket->write(command.toUtf8());
    _socket->flush();

}

void TelnetClient::processData()
{
    if (_status !=1) return;

    QString line;

    bool endOfLine = false;

    while ((!endOfLine))
    {
        if(_status==1)
        {
            char ch;
            if(_socket->bytesAvailable()>0)
            {
                int bytesRead = _socket->read(&ch, sizeof(ch));
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

    }


    emit haveProperty(line);
}


