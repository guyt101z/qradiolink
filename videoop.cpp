#include "videoop.h"

using namespace cv;

VideoOp::VideoOp(QObject *parent) :
    QObject(parent)
{


}

void VideoOp::run()
{
    VideoCapture cap(1); // open the video camera no. 0

    if (!cap.isOpened())  // if not success, exit program
    {
        qDebug() << "Cannot open the video cam";

    }
    namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
    while(true)
    {
        Mat frame;
        Mat yuvframe;
        bool bSuccess = cap.read(frame); // read a new frame from video

        if (!bSuccess) //if not success, break loop
        {
            qDebug() << "Cannot read a frame from video stream";
            break;
        }
        //cout << frame.data << " " << frame.total() * frame.channels() << endl;
        cvtColor(frame, yuvframe, CV_RGB2YCrCb); //CV_BGR2YCrCb
        imshow("MyVideo", yuvframe); //show the frame in "MyVideo" window
    }
}
