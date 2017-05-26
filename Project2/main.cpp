#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <cv.h>
#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;

int main()
{
	Mat srcImg = imread("2.jpg");
	Mat addImg = imread("2.png");
	
	srcImg.convertTo(srcImg, CV_32F, 1.0f/255);
	addImg.convertTo(addImg, CV_32F, 1.0f/255);
	resize(addImg, addImg, Size(100, 50));
	Mat gradient(addImg.size(), CV_32FC3);

	for (int i = 1; i < addImg.rows - 1; i++){
		for (int j = 1; j < addImg.cols - 1; j++){
			//printf("i=%d j=%d\n", i, j);
			gradient.at<Vec3f>(i, j)[0] = addImg.at<Vec3f>(i - 1, j)[0]
				+ addImg.at<Vec3f>(i + 1, j)[0]
				+ addImg.at<Vec3f>(i, j - 1)[0]
				+ addImg.at<Vec3f>(i, j + 1)[0]
				- 4 * addImg.at<Vec3f>(i, j)[0];
			gradient.at<Vec3f>(i, j)[1] = addImg.at<Vec3f>(i - 1, j)[1]
				+ addImg.at<Vec3f>(i + 1, j)[1]
				+ addImg.at<Vec3f>(i, j - 1)[1]
				+ addImg.at<Vec3f>(i, j + 1)[1]
				- 4 * addImg.at<Vec3f>(i, j)[1];
			gradient.at<Vec3f>(i, j)[2] = addImg.at<Vec3f>(i - 1, j)[2]
				+ addImg.at<Vec3f>(i + 1, j)[2]
				+ addImg.at<Vec3f>(i, j - 1)[2]
				+ addImg.at<Vec3f>(i, j + 1)[2]
				- 4 * addImg.at<Vec3f>(i, j)[2];
			
		}
	}

	
	Mat tmp = srcImg(Rect(170, 150, 100, 50));
	//addImg.copyTo(tmp);
	imshow("test1", srcImg);
	for (int k = 0; k <9000; k++){
	//	printf("%d ",k);
		for (int i = 1; i < tmp.rows - 1; i++){
			for (int j = 1; j < tmp.cols - 1; j++){
			//	printf("%.6f\n", tmp.at<Vec3f>(i, j)[0]);
				//printf("%f %f %f,", tmp.at<Vec3f>(i, j)[0], tmp.at<Vec3f>(i, j)[1], tmp.at<Vec3f>(i, j)[2]);
				tmp.at<Vec3f>(i, j)[0] = (tmp.at<Vec3f>(i - 1, j)[0]
					+ tmp.at<Vec3f>(i + 1, j)[0]
					+ tmp.at<Vec3f>(i, j - 1)[0]
					+ tmp.at<Vec3f>(i, j + 1)[0]
					- gradient.at<Vec3f>(i, j)[0]) / 4;
				tmp.at<Vec3f>(i, j)[1] = (tmp.at<Vec3f>(i - 1, j)[1]
					+ tmp.at<Vec3f>(i + 1, j)[1]
					+ tmp.at<Vec3f>(i, j - 1)[1]
					+ tmp.at<Vec3f>(i, j + 1)[1]
					- gradient.at<Vec3f>(i, j)[1]) / 4;
				tmp.at<Vec3f>(i, j)[2] = (tmp.at<Vec3f>(i - 1, j)[2]
					+ tmp.at<Vec3f>(i + 1, j)[2]
					+ tmp.at<Vec3f>(i, j - 1)[2]
					+ tmp.at<Vec3f>(i, j + 1)[2]
					- gradient.at<Vec3f>(i, j)[2])/4;
				//printf("%f %f %f\n", tmp.at<Vec3f>(i, j)[0], tmp.at<Vec3f>(i, j)[1], tmp.at<Vec3f>(i, j)[2]);
			}
		}
	}
	
    imshow("test", srcImg);
	waitKey(0);
}