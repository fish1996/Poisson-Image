#ifndef BACKWINDOW_H
#define BACKWINDOW_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <QDebug>
#include <QWidget>
#include <vector>
#include <QMouseEvent>
#include <QTimer>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include "poisson.h"
using namespace std;
using namespace cv;

class backwindow : public QWidget
{
    Q_OBJECT
public:
    backwindow(QWidget *parent = 0);
    ~backwindow();

private:
    Mat image;
    QPixmap pix;

    bool flag; //开始绘制
    bool flags; //是否完成绘制
    int inum; //表示封闭区域数量-1

    int w; //实际变成的大小
    int h;
    Point pt_origin;
    Point pt_Cur;
    vector< vector<Point>>  co_ordinates;

    int choosenum;

    QTimer *timer; //计时器

public:
    int width;
    int height;
    int fitw; //用于记录切割后掩码图的原宽
    int fith;
    Mat fitsrc;//被割过的原图
    Mat fitmask;//fitsrc对应的掩码图
    Mat srcImg;
    Mat maskImg;//和原图大小一样的掩码图

    Mat *alphaimg; //用于图像边缘透明

    Poisson* poisson;

    void createAlphaMat(Mat *mat);

    Mat PutMask();
    Mat PutSrc();
    Mat PutFitSrc();
    Mat PutFitMask();
    Mat PutFitALP();
    void GetMask();
    void GetMaskAndSrc(Mat src, Mat mask);
    void GetChoose(int choose);
    void ChangeSize(int sw, int sh);

protected:
    void paintEvent(QPaintEvent *); //绘制函数
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    Mat resizemat(Mat img); //适当调整图像大小

public:
    void start(Type type);
    void open(QString filename);
    void clear();
    void finish();
};

#endif // BACKWINDOW_H
