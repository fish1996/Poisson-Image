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
	Mat* srcImg; //原图
	Mat* addImg; //加上的图
	Mat* maskImg; //掩码图

	Mat* magnitude;

	Mat tmp; //临时图，存的信息是ROI图像（感兴趣区域）,矩形

	Color color;//颜色

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

	void cal_light(float& g,float m);
	void multi_light();
	void multi_color();//颜色通道乘法
	void cal_gradient(Mat* img);//求导
	void cal_magnitude();
	void cal_magnitude(int i, int j, int k);

	void print(Mat* temp);//矩阵输出
	void solve_poisson1();//求解泊松方法1
	void solve_poisson2();//求解泊松方法2

	void init();

public:
	Poisson();
	void set(Mat* src, Mat* mask, Color c, int times, int x, int y, int w, int h);
	void set(Mat* src, Color c, int times, int x, int y, int w, int h);
	void set(Mat* src, Mat* mask, float alpha, float beta, int times, int x, int y, int w, int h);
	void set(Mat* src, float a, float b, int times, int x, int y, int w, int h);
	void set(Mat* src, Mat* add, Mat* mask, int times, int x, int y, int w, int h, float min = 0, float max = 1, int factor = 10);
	void set(Mat* src, Mat* add, int times, int x, int y, int w, int h, float min = 0, float max = 1, int factor = 10);

	Mat* run(Type type);

};
#endif