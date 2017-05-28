#include "poisson.h"

int main()
{
	Poisson p;
	Mat src = imread("2.jpg");
	Mat dst = imread("2.png");

	p.set(&src,&dst,300,170, 150, 100, 50);
	Mat* ans = p.run(GRADIENT);
	imshow("1", *ans);
	waitKey(10);
	system("pause");
}