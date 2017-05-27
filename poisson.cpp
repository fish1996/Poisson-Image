#include "poisson.h"
#include <math.h>
void poisson::mkTempCos(int m, int n)
{
	int factor = 10;
	int max = 1;
	int min = 0;
	Mat* weight = new Mat[factor];
	Mat temp(Size(m,n), CV_8UC3, Scalar(0));
	for (int i = 0; i < factor; i++) {
		weight[i] = (max - min)*(cos(PI*i / factor) + 1);
	}

}

void poisson::run()
{
	srcImg->convertTo(*srcImg, CV_32F, 1.0f / 255);
	addImg->convertTo(*addImg, CV_32F, 1.0f / 255);
	gradient = new Mat((*addImg).size(), CV_32FC3);
	for (int i = 1; i < addImg->rows - 1; i++) {
		for (int j = 1; j < addImg->cols - 1; j++) {
			//printf("i=%d j=%d\n", i, j);
			gradient->at<Vec3f>(i, j)[0] = addImg->at<Vec3f>(i - 1, j)[0]
				+ addImg->at<Vec3f>(i + 1, j)[0]
				+ addImg->at<Vec3f>(i, j - 1)[0]
				+ addImg->at<Vec3f>(i, j + 1)[0]
				- 4 * addImg->at<Vec3f>(i, j)[0];
			gradient->at<Vec3f>(i, j)[1] = addImg->at<Vec3f>(i - 1, j)[1]
				+ addImg->at<Vec3f>(i + 1, j)[1]
				+ addImg->at<Vec3f>(i, j - 1)[1]
				+ addImg->at<Vec3f>(i, j + 1)[1]
				- 4 * addImg->at<Vec3f>(i, j)[1];
			gradient->at<Vec3f>(i, j)[2] = addImg->at<Vec3f>(i - 1, j)[2]
				+ addImg->at<Vec3f>(i + 1, j)[2]
				+ addImg->at<Vec3f>(i, j - 1)[2]
				+ addImg->at<Vec3f>(i, j + 1)[2]
				- 4 * addImg->at<Vec3f>(i, j)[2];
		}
	}
	//170, 150, 100, 50
	tmp = (*srcImg)(Rect(begin, end, width, height));
	for (int k = 0; k <9000; k++) {
		for (int i = 1; i < tmp.rows - 1; i += 2) {
			for (int j = 1; j < tmp.cols - 1; j += 2) {
				calculate(i, j);
			}
		}
		for (int i = 1; i < tmp.rows - 1; i += 2) {
			for (int j = 2; j < tmp.cols - 1; j += 2) {
				calculate(i, j);
			}
		}
		for (int i = 2; i < tmp.rows - 1; i += 2) {
			for (int j = 1; j < tmp.cols - 1; j += 2) {
				calculate(i, j);
			}
		}
		for (int i = 2; i < tmp.rows - 1; i += 2) {
			for (int j = 2; j < tmp.cols - 1; j += 2) {
				calculate(i, j);
			}
		}
	}
}

void poisson::calculate(int i,int j)
{
	float omega = 0.1f;
	tmp.at<Vec3f>(i, j)[0] = (tmp.at<Vec3f>(i - 1, j)[0]
		+ tmp.at<Vec3f>(i + 1, j)[0]
		+ tmp.at<Vec3f>(i, j - 1)[0]
		+ tmp.at<Vec3f>(i, j + 1)[0]
		- gradient->at<Vec3f>(i, j)[0]) / 4;
	tmp.at<Vec3f>(i, j)[1] = (tmp.at<Vec3f>(i - 1, j)[1]
		+ tmp.at<Vec3f>(i + 1, j)[1]
		+ tmp.at<Vec3f>(i, j - 1)[1]
		+ tmp.at<Vec3f>(i, j + 1)[1]
		- gradient->at<Vec3f>(i, j)[1]) / 4;
	tmp.at<Vec3f>(i, j)[2] = (tmp.at<Vec3f>(i - 1, j)[2]
		+ tmp.at<Vec3f>(i + 1, j)[2]
		+ tmp.at<Vec3f>(i, j - 1)[2]
		+ tmp.at<Vec3f>(i, j + 1)[2]
		- gradient->at<Vec3f>(i, j)[2]) / 4;
}
void poisson::set(Mat* src, Mat* add,
	int x, int y, int w, int h)
{
	width = w;
	height = h;
	begin = x;
	end = y;
	*srcImg = src->clone();
	*addImg = add->clone();
}