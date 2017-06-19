#ifndef CHOOSEIMG_H
#define CHOOSEIMG_H

#include <cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <QLabel>
#include <QPoint>
#include <QMouseEvent>
#include <QTimer>
#include <QDebug>
#include <QPixmap>
using namespace std;
using namespace cv;

class ChooseImg: public QLabel
{
    Q_OBJECT
public:
    ChooseImg();
    ~ChooseImg(){}
    void CanMoveImgP();
    void CanMoveImgR();
    void GetImg(Mat fitImg);
    void ChangeW(float cw);
    void ChangeH(float ch);
    void SetImg();
    int w;
    int h;
    int sw;
    int sh;
    QPoint ImgPos;
    QPixmap pix;

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private slots:
    //void mytest();

private:
    QPoint old;
    bool IsMove;
    bool CanMove;
};
#endif // CHOOSEIMG_H
