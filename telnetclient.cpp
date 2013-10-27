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

void TelnetClient::disconnectFromFGFS()
{
    _socket->disconnectFromHost();
    _status=0;
    _connection_tries=0;
    emit connectionFailure();
}


void TelnetClient::setProperty(QString prop_name, QString value)
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

void TelnetClient::getProperty(QString prop_name)
{

    QString command = "get " + prop_name +CRLF;
    int check = _socket->write(command.toLatin1());
    if(check==-1)
    {
        qDebug() << "Command was not written, an error has occured";
    }
    else if(check < qstrlen(command.toLatin1()))
    {
        qDebug() << "Fewer than required bytes written to socket";
    }

    _socket->flush();

}

void TelnetClient::cd(QString dir)
{
    qDebug() << "dir: " << dir;
    QString command = "cd " + dir + " " +CRLF;
    _socket->write(command.toUtf8());
    _socket->flush();
}


void TelnetClient::runCmd(QString cmd)
{
    QString command = "run " + cmd + CRLF;
    _socket->write(command.toUtf8());
    _socket->flush();

}

void TelnetClient::dataMode()
{
    QString command = "data " + CRLF;
    _socket->write(command.toUtf8());
    _socket->flush();

}

void TelnetClient::promptMode()
{
    QString command = "prompt " + CRLF;
    _socket->write(command.toUtf8());
    _socket->flush();

}
