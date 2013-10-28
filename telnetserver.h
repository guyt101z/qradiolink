#ifndef TELNETSERVER_H
#define TELNETSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QTime>
#include <QAbstractSocket>
#include <QVector>
#include <QHostAddress>
#include <QDebug>
#include <QCoreApplication>

class TelnetServer : public QObject
{
    Q_OBJECT
public:
    explicit TelnetServer(QObject *parent = 0);
    ~TelnetServer();
    void stop();

signals:
    void finished();
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
    int _status;
    bool _stop;
    QHostAddress _hostname;
    unsigned _listen_port;
    QVector<QTcpSocket*> _unconnected_clients;
    QVector<QTcpSocket*> _connected_clients;
    
};

#endif // TELNETSERVER_H
