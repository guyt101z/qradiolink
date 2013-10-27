#include <QCoreApplication>
#include <QThread>
#include <dtmfdecoder.h>
#include <QObject>

int main(int argc, char *argv[])
{
    int detector(char* name);
    QCoreApplication a(argc, argv);
    //_start_time= QDateTime::currentDateTime().toString("d/MMM/yyyy hh:mm:ss");
    QThread *t= new QThread;
    DtmfDecoder *decoder = new DtmfDecoder;
    decoder->moveToThread(t);
    //connect(decoder, SIGNAL(haveMobilePosition(double,double)), this, SLOT(moveMobile(double,double)));
    //connect(decoder, SIGNAL(haveSignalReading(double, double, int,QString,double,Signal*)), this, SLOT(showSignalReading(double, double, int,QString,double,Signal*)));

    QObject::connect(t, SIGNAL(started()), decoder, SLOT(run()));
    QObject::connect(decoder, SIGNAL(finished()), t, SLOT(quit()));
    QObject::connect(decoder, SIGNAL(finished()), decoder, SLOT(deleteLater()));
    QObject::connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));

    t->start();

    return a.exec();
}
