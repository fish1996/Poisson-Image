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
#include "color.h"

using namespace cv;
class Poisson
{
private:
    float PI;
    Mat* gradient; //梯度矩阵
    Mat* gradientX; //梯度矩阵(x方向）
    Mat* gradientY; //梯度矩阵（y方向）
    Mat* srcImg; //原图
    Mat* addImg; //加上的图
    Mat* maskImg; //掩码图

    Mat* magnitude;

    Mat tmp; //临时图，存的信息是ROI图像（感兴趣区域）,矩形
    Mat* srcgradient; //梯度矩阵 (原图)
    Mat* srcgradientX; //梯度矩阵 (原图x方向)
    Mat* srcgradientY; //梯度矩阵 (原图y方向)

    Color color;//颜色

    float low;
    float high;
    int ksize; //纹理平滑的算子大小
                //选区位置
    int beginw;
    int beginh;
    int width;
    int height;

    float alpha;
    float beta;

    int iterTimes;// iterTimes迭代次数
    int factor;
    float max;
    float min;
    bool isMask;//是否有掩码图
    Mat* mkTempCos(int m, int n);//优化函数
    void calculate(int i, int j);//求解泊松方程
    void subtract(int num, Mat* mat);//数字减去矩阵
    float pow2(float x);

    void run_normal();
    void run_gradient();
    void run_color();
    void run_texture();
    void run_light();
    void run_mixed();

    void cal_light(float& g,float m);
    void multi_light();
    void multi_color();//颜色通道乘法
    void canny_texture(Mat out); //canny算子调整边缘
    void cal_gradient(Mat* img);//求导
    void cal_magnitude();
    void cal_magnitude(int i, int j, int k);

    void cal_gradientX(Mat* img,Mat* gradientX);//求导
    void cal_gradientY(Mat* img,Mat* gradientY);//求导
    void cal_LaplacianX(Mat* img,Mat* gradientX);
    void cal_LaplacianY(Mat* img,Mat* gradientY);

    void print(Mat* temp);//矩阵输出
    void solve_poisson1();//求解泊松方法1
    void solve_poisson2();//求解泊松方法2

    void run_gray();

    void init();

public:
    Poisson();//src是原图，add和mask都是处理后得到的
    void set(Mat* src, Mat* mask, Color c, int times, int x, int y, int w, int h);
    void set(Mat* src, Color c, int times, int x, int y, int w, int h);
    void set(Mat* src, Mat* mask, float alpha, float beta, int times, int x, int y, int w, int h);
    void set(Mat* src, float a, float b, int times, int x, int y, int w, int h);
    void set(Mat* src, Mat* add, Mat* mask, int times, int x, int y, int w, int h, float min = 0, float max = 1, int factor = 10);
    void set(Mat* src, Mat* add, int times, int x, int y, int w, int h, float min = 0, float max = 1, int factor = 10);
    void set(Mat* src, Mat* mask, float lt, float ht, int size, int times, int x, int y, int w, int h);

    Mat* run(Type type);

};
#endif
