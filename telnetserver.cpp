// Written by Adrian Musceac YO8RZZ at gmail dot com, started October 2013.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include "telnetserver.h"

static QString CRLF ="\r\n";

TelnetServer::TelnetServer(DatabaseApi *db, QObject *parent) :
    QObject(parent)
{
    _hostname = QHostAddress::Any;
    _listen_port = CONTROL_PORT;
    _stop=false;
    _server = new QTcpServer;
    _db = db;
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
    qDebug() << "Incoming connection" << socket->peerAddress().toString();
    if(socket->state() == QTcpSocket::ConnectedState)
        qDebug() << "Connection established";

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
    qDebug() << "Connection established: " << socket->peerAddress().toString() << ":" << socket->peerPort();
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
                    socket->read(&ch, sizeof(ch)); // for newline

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
    qDebug() << "Received message from: " << socket->peerAddress().toString() << line;
    QString response = processCommand(line);
    socket->write(response.toUtf8());

}

QString TelnetServer::processCommand(QString command)
{
    QStringList pre = command.split(";");
    if(pre.size()<2)
    {
        qDebug() << "Invalid command " << command;
        return "";
    }
    if(pre[0]=="PARAMETERS")
    {
        Station* s=_db->get_station_by_id(pre[1].toInt());
        QString response ="PARAMETERS;" +
                QString::number(s->_in_call) +";"+
                s->_conference_id +
                ";" + QString::number(s->_called_by) + CRLF;
        return response;
    }
    else if(pre[0]=="JOIN")
    {
        QString response = "JOIN;1" + CRLF;
        emit joinConference(pre[1],pre[2].toInt(),pre[3].toInt());
        return response;
    }
    else if(pre[0]=="LEAVE")
    {
        QString response = "LEAVE;1" + CRLF;
        emit leaveConference(pre[1],pre[2].toInt(),pre[3].toInt());
        return response;
    }
    else
    {
        return "COMMAND;-1" + CRLF;
    }
}
