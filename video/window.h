#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class window : public QMainWindow
{
    Q_OBJECT
public:
    explicit window(QWidget *parent = 0);
    
signals:
    
public slots:
    void refreshImage(cv::Mat frame);

private:
    QLabel *_label;
    
};

#endif // WINDOW_H
