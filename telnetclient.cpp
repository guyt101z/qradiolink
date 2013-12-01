#include "telnetclient.h"


static QString CRLF ="\r\n";

TelnetClient::TelnetClient(QObject *parent) :
    QObject(parent)
{
    _connection_tries=0;
    _status=0;
    _hostname = "localhost";
    _port= CONTROL_PORT;
    _socket = new QTcpSocket;
    QObject::connect(_socket,SIGNAL(error(QAbstractSocket::SocketError )),this,SLOT(connectionFailed(QAbstractSocket::SocketError)));
    QObject::connect(_socket,SIGNAL(connected()),this,SLOT(connectionSuccess()));
    QObject::connect(_socket,SIGNAL(readyRead()),this,SLOT(processData()));

}


TelnetClient::~TelnetClient()
{
    _socket->disconnectFromHost();
    delete _socket;
}


void TelnetClient::connectionSuccess()
{
    qDebug() << "Successfull outgoing connection";
    _status=1;
    _connection_tries=0;
    emit connectedToHost();
}


void TelnetClient::connectionFailed(QAbstractSocket::SocketError error)
{
    _status=0;
    qDebug() << "Outgoing connection failed" << _socket->errorString();
    _connection_tries++;
    if(_connection_tries < 10)
    {
        this->connectHost(_hostname,_port);
    }
    else
    {

        emit connectionFailure();
    }

}


void TelnetClient::connectHost(const QString &host, const unsigned &port)
{
    qDebug() << "trying " << host;
    if(_status==1) return;
    _socket->connectToHost(host, port);
    _hostname = host;
    _port = port;

}

void TelnetClient::disconnectHost()
{
    if(_status==0)
        return;
    _socket->disconnectFromHost();
    _status=0;
    _connection_tries=0;
    emit disconnectedFromHost();
}


void TelnetClient::send(QString prop_name, QString value)
{
    QString command = prop_name + ";" + value + CRLF;
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
                        _socket->read(&ch, sizeof(ch)); // for newline

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

    qDebug() << "Received message from " << _socket->peerAddress().toString() << " :" << line;
    emit haveMessage(line);
}


