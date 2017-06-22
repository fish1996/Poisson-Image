#ifndef LABTRANSFER_H
#define LABTRANSFER_H
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <QDebug>
#include <cmath>
using namespace std;
using namespace cv;

class LABtransfer
{
public:
    LABtransfer();
    ~LABtransfer();
private:
     Mat source;
     Mat target;
     Mat sourcelab;
     Mat targetlab;
     Mat trans;
     Mat *transfer;

     //计算得到的均值和方差
     Vector<double> srcMeans;
     Vector<double> targetMeans;
     Vector<double> srcVariances;
     Vector<double> targetVariances;

     void image_stats();
     void color_transfer();
     Mat RGBToLab(Mat m);
     Mat LabToRGB(Mat m);
     Vector<double> computeMeans(Mat m);
     Vector<double> computeVariances(Mat m, Vector<double> means);

public:
     void Getimg(Mat* src, Mat* tar);
     Mat OutPut();

};

#endif // LABTRANSFER_H
