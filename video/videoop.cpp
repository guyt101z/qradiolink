#include "videoop.h"



VideoOp::VideoOp(QObject *parent) :
    QObject(parent)
{


}

void VideoOp::run()
{
    cv::VideoCapture cap(1); // open the video camera no. 1
    bool can_capture = true;
    if (!cap.isOpened())  // if not success,
    {
        qDebug() << "Cannot open the video cam";
        can_capture = false;
    }
    //cv::namedWindow("MyVideo2",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"


    while(true)
    {

        usleep(10000);
        if(!can_capture)
            continue;
        cv::Mat frame;
        cv::Mat yuvframe;

        bool bSuccess = cap.read(frame); // read a new frame from video

        if (!bSuccess) //if not success,
        {
            qDebug() << "Cannot read a frame from video stream";
            continue;
        }
        //qDebug() << frame.data << " " << frame.total() * frame.channels() ;

        cv::cvtColor(frame, yuvframe, CV_BGR2RGB); //CV_BGR2YCrCb
        int framesize = yuvframe.total() * yuvframe.elemSize();
        uchar *data = yuvframe.data;
        //emit videoFrameData(data, framesize);
        //cv::imshow("MyVideo2", yuvframe); //show the frame in "MyVideo" window

        emit imageData(yuvframe);


    }
    emit finished();
}


