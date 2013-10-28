#include <QCoreApplication>
#include <QThread>
#include <QObject>
#include <QDateTime>
#include <QString>
#include "dtmfdecoder.h"
#include "serverwrapper.h"
#include "speech.h"


int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    QString _start_time= QDateTime::currentDateTime().toString("d/MMM/yyyy hh:mm:ss");

    QThread *t1= new QThread;
    DtmfDecoder *decoder = new DtmfDecoder;
    decoder->moveToThread(t1);

    QObject::connect(t1, SIGNAL(started()), decoder, SLOT(run()));
    QObject::connect(decoder, SIGNAL(finished()), t1, SLOT(quit()));
    QObject::connect(decoder, SIGNAL(finished()), decoder, SLOT(deleteLater()));
    QObject::connect(t1, SIGNAL(finished()), t1, SLOT(deleteLater()));
    t1->start();


    QThread *t2= new QThread;
    ServerWrapper *telnet_server_wrapper = new ServerWrapper;
    telnet_server_wrapper->moveToThread(t2);

    QObject::connect(t2, SIGNAL(started()), telnet_server_wrapper, SLOT(run()));
    QObject::connect(telnet_server_wrapper, SIGNAL(finished()), t2, SLOT(quit()));
    QObject::connect(telnet_server_wrapper, SIGNAL(finished()), telnet_server_wrapper, SLOT(deleteLater()));
    QObject::connect(t2, SIGNAL(finished()), t2, SLOT(deleteLater()));
    t2->start();

    return a.exec();
}
