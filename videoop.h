#ifndef VIDEOOP_H
#define VIDEOOP_H

#include <QObject>
#include <QDebug>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class VideoOp : public QObject
{
    Q_OBJECT
public:
    explicit VideoOp(QObject *parent = 0);
    
signals:
    
public slots:
    void run();
    
};

#endif // VIDEOOP_H
