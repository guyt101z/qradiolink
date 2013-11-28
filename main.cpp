#include <QCoreApplication>
#include <QThread>
#include <QObject>
#include <QDateTime>
#include <QString>
#include "dtmfdecoder.h"
#include "databaseapi.h"
#include "serverwrapper.h"
#include "speech.h"
#include "audioclient.h"
#include "controller.h"
#include "festival/festival.h"


int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    QString _start_time= QDateTime::currentDateTime().toString("d/MMM/yyyy hh:mm:ss");
    DatabaseApi *db= new DatabaseApi;

    Controller *controller = new Controller(db);

    QThread *t1= new QThread;
    DtmfDecoder *decoder = new DtmfDecoder;
    decoder->moveToThread(t1);
    QObject::connect(decoder,SIGNAL(haveCall(QVector<char>*)),controller,SLOT(haveCall(QVector<char>*)));
    QObject::connect(decoder,SIGNAL(haveCommand(QVector<char>*)),controller,SLOT(haveCommand(QVector<char>*)));
    QObject::connect(controller,SIGNAL(readyInput()),decoder,SLOT(resetInput()));
    QObject::connect(t1, SIGNAL(started()), decoder, SLOT(run()));
    QObject::connect(decoder, SIGNAL(finished()), t1, SLOT(quit()));
    QObject::connect(decoder, SIGNAL(finished()), decoder, SLOT(deleteLater()));
    QObject::connect(t1, SIGNAL(finished()), t1, SLOT(deleteLater()));
    t1->start();


    QThread *t2= new QThread;
    ServerWrapper *telnet_server_wrapper = new ServerWrapper(db);
    telnet_server_wrapper->moveToThread(t2);
    QObject::connect(controller,SIGNAL(speak(QString)),telnet_server_wrapper,SLOT(addSpeech(QString)));
    QObject::connect(telnet_server_wrapper,SIGNAL(joinConference(QString,QString,int)),controller,SLOT(joinConference(QString,QString,int)));
    QObject::connect(t2, SIGNAL(started()), telnet_server_wrapper, SLOT(run()));
    QObject::connect(telnet_server_wrapper, SIGNAL(finished()), t2, SLOT(quit()));
    QObject::connect(telnet_server_wrapper, SIGNAL(finished()), telnet_server_wrapper, SLOT(deleteLater()));
    QObject::connect(t2, SIGNAL(finished()), t2, SLOT(deleteLater()));
    t2->start();


    return a.exec();
}
