#ifndef TELNETCLIENT_H
#define TELNETCLIENT_H

#include <QTcpSocket>
#include <QAbstractSocket>
#include <QHostAddress>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QTime>
#include <QCoreApplication>
#include <QLatin1String>

class TelnetClient : public QObject
{
    Q_OBJECT
public:
    explicit TelnetClient(QObject *parent = 0);
    ~TelnetClient();

    void send(QString prop_name, QString value);

    void disconnectFromHost();
    unsigned inline status() {return _status;}

public slots:
    void processData();
    void connectHost(const QString &host, const unsigned &port);

signals:
    void connectionFailure();
    void connectedToHost();
    void haveMessage(QString prop);

private:
    QTcpSocket *_socket;
    unsigned _connection_tries;
    unsigned _status;
    QString _hostname;
    unsigned _port;


private slots:
    void connectionSuccess();
    void connectionFailed(QAbstractSocket::SocketError error);

};

#endif // TELNETCLIENT_H
