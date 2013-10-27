#ifndef TELNETCLIENT_H
#define TELNETCLIENT_H

#include <QTcpSocket>
#include <QAbstractSocket>
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

    void setProperty(QString prop_name, QString value);

    void runCmd(QString cmd);
    void cd(QString dir);
    void dataMode();
    void promptMode();
    void disconnectFromFGFS();
    unsigned inline status() {return _status;}

public slots:
    void processData();
    void connectToHost(const QString &host, const unsigned &port);
    void getProperty(QString prop_name);

signals:
    void connectionFailure();
    void connectedToHost();
    void haveProperty(QString prop);

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
