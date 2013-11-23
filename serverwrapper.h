#ifndef SERVERWRAPPER_H
#define SERVERWRAPPER_H

#include <QObject>
#include <unistd.h>
#include "telnetserver.h"
#include "speech.h"

class ServerWrapper : public QObject
{
    Q_OBJECT
public:
    explicit ServerWrapper(QObject *parent = 0);
    void stop();

signals:
    void finished();
public slots:
    void run();
private:
    bool _stop;
};

#endif // SERVERWRAPPER_H
