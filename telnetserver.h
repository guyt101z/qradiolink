#ifndef TELNETSERVER_H
#define TELNETSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QStringList>
#include <QTime>
#include <QAbstractSocket>
#include <QVector>
#include <QHostAddress>
#include <QDebug>
#include <QCoreApplication>
#include "databaseapi.h"

class TelnetServer : public QObject
{
    Q_OBJECT
public:
    explicit TelnetServer(DatabaseApi *db, QObject *parent = 0);
    ~TelnetServer();
    void stop();

signals:
    void finished();
    void joinConference(QString number, QString ip, int id);
public slots:
    void run();

private slots:
    void getConnection();
    void connectionFailed(QAbstractSocket::SocketError error);
    void connectionSuccess();
    void processData();

private:
    QTcpServer *_server;
    QTcpSocket _socket;
    DatabaseApi *_db;
    int _status;
    bool _stop;
    QHostAddress _hostname;
    unsigned _listen_port;
    QVector<QTcpSocket*> _unconnected_clients;
    QVector<QTcpSocket*> _connected_clients;
    QString processCommand(QString command);

};

#endif // TELNETSERVER_H
