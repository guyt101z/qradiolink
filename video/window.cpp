#include "window.h"

window::window(QWidget *parent) :
    QMainWindow(parent)
{

    QLabel *w = new QLabel("Video",this);
    this->setFixedHeight(480);
    this->setFixedWidth(600);
    w->setFixedHeight(480);
    w->setFixedWidth(600);
    //w->setAutoFillBackground(true);
    //w->setVisible(true);
    _label = w;

    w->show();
}

void window::refreshImage(cv::Mat frame)
{
    QImage imgIn= QImage(reinterpret_cast<uchar*>(frame.data), frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
    _label->setPixmap(QPixmap::fromImage(imgIn));
}
