#ifndef POISSON_H
#define POISSON_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <cv.h>
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "type.h"
#include <fstream>
using namespace cv;
class Poisson
{
private:
    float PI;
    Mat* gradient;
    Mat* srcImg;
    Mat* addImg;
    Mat* maskImg;

    Mat dst;
    Mat tmp;

    int begin;
    int end;
    int width;
    int height;
    int iterTimes;
    int factor;
    float max;
    float min;
    bool isMask;
    Mat* mkTempCos(int m, int n);
    void calculate(int i,int j);
    void subtract(int num,Mat* mat);
    void run_normal();
    void run_gradient();
    void run_color();
    void run_texture();
    void cal_gradient();

    void print(Mat* temp);
    std::ofstream out;

public:
    Poisson();
    void set(Mat* src, Mat* add, Mat* mask,int times, int x, int y, int w, int h, float min = 0, float max = 1, int factor = 10);
    void set(Mat* src, Mat* add, int times, int x, int y, int w, int h, float min = 0, float max = 1, int factor = 10);
    // iterTimes迭代次数
    Mat* run(Type type);

};
#endif
