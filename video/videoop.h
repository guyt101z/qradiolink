#ifndef VIDEOOP_H
#define VIDEOOP_H

#include <QObject>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <unistd.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class VideoOp : public QObject
{
    Q_OBJECT
public:
    explicit VideoOp(QObject *parent = 0);
    
signals:
    void finished();
    void videoFrameData(uchar *data, int size);
    void imageData(cv::Mat frame);
public slots:
    void run();

private:

};

#endif // VIDEOOP_H
