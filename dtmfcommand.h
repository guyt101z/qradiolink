#ifndef DTMFCOMMAND_H
#define DTMFCOMMAND_H

#include <QObject>

class DtmfCommand : public QObject
{
    Q_OBJECT
public:
    explicit DtmfCommand(QObject *parent = 0);

signals:

public slots:

};

#endif // DTMFCOMMAND_H
