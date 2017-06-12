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

    /*int *x; //鼠标点击获取横坐标数组
    int *y; //鼠标点击获取纵坐标数组
    int num; //点数量*/
    bool flag; //开始绘制
    bool flags; //是否完成绘制
    int inum; //表示封闭区域数量-1

    Mat srcImg;
    Mat maskImg;
    Poisson* poisson;
    Point pt_origin;
    Point pt_Cur;
    vector< vector<Point>>  co_ordinates;

public:
    int width;
    int height;

protected:
    void paintEvent(QPaintEvent *); //绘制函数
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

public:
    void start();
    void open(QString filename);
    void clear();
};

#endif // BACKWINDOW_H
