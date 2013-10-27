#ifndef DTMFDECODER_H
#define DTMFDECODER_H

#include <QObject>
#include <QtDebug>
#include "audiointerface.h"
#include "dtmf.h"

class DtmfDecoder : public QObject
{
    Q_OBJECT
public:
    explicit DtmfDecoder(QObject *parent = 0);
    ~DtmfDecoder();
    void stop();
    
signals:
    void finished();
    
public slots:
    void run();

private:
    bool _stop;
    
};

#endif // DTMFDECODER_H
