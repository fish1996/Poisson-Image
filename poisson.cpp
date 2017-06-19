#include "poisson.h"
#include <iostream>
#include <QDebug>

Poisson::Poisson()
{
    PI = 3.14159;
}

void Poisson::init()
{
    gradientX = nullptr;
    gradientY = nullptr;
    srcgradientX = nullptr;
    srcgradientY = nullptr;
    srcgradient = nullptr;
    srcImg = nullptr;
    addImg = nullptr;
    maskImg = nullptr;
    magnitude = nullptr;
}

void Poisson::print(Mat* temp)
{
    printf("%d %d\n", temp->rows, temp->cols);
    for (int i = 0; i < temp->rows; i++) {
        printf("row %d: ", i + 1);
        for (int j = 0; j < temp->cols; j++) {
            printf("%.2f ", temp->at<Vec3f>(i, j)[0]);
        }
        printf("\n");
    }
}

Mat* Poisson::mkTempCos(int m, int n)
{
    int _i = 0, _j = 0;
    float* weight = new float[factor];
    Mat* temp = new Mat(Size(n, m), CV_32FC3, Scalar(0));
    for (int i = 0; i < factor; i++) {
        weight[i] = 1.0*(max - min)*(cos(PI*i / factor) + 1) / 2 + min;
    }

    for (int i = factor; i < m - factor; i++) {
        _j = 0;
        for (int j = 0; j < factor; j++) {
            temp->at<Vec3f>(i, j)[0] = weight[_j];
            temp->at<Vec3f>(i, j)[1] = weight[_j];
            temp->at<Vec3f>(i, j)[2] = weight[_j];
            _j++;
        }
    }

    for (int i = factor; i < m - factor; i++) {
        _j = 1;
        for (int j = n - factor; j < n; j++) {
            temp->at<Vec3f>(i, j)[0] = weight[factor - _j];
            temp->at<Vec3f>(i, j)[1] = weight[factor - _j];
            temp->at<Vec3f>(i, j)[2] = weight[factor - _j];
            _j++;
        }
    }

    _i = 0;
    for (int i = 0; i < factor; i++) {
        for (int j = factor; j < n - factor; j++) {
            temp->at<Vec3f>(i, j)[0] = weight[_i];
            temp->at<Vec3f>(i, j)[1] = weight[_i];
            temp->at<Vec3f>(i, j)[2] = weight[_i];
        }
        _i++;
    }

    _i = 1;
    for (int i = m - factor; i < m; i++) {
        for (int j = factor; j < n - factor; j++) {
            temp->at<Vec3f>(i, j)[0] = weight[factor - _i];
            temp->at<Vec3f>(i, j)[1] = weight[factor - _i];
            temp->at<Vec3f>(i, j)[2] = weight[factor - _i];
        }
        _i++;
    }

    for (int i = 0; i < factor; i++) {
        for (int j = 0; j < factor; j++) {
            if (i > j) {
                temp->at<Vec3f>(i, j)[0] = weight[j];
                temp->at<Vec3f>(i, j)[1] = weight[j];
                temp->at<Vec3f>(i, j)[2] = weight[j];
            }
            else {
                temp->at<Vec3f>(i, j)[0] = weight[i];
                temp->at<Vec3f>(i, j)[1] = weight[i];
                temp->at<Vec3f>(i, j)[2] = weight[i];
            }
        }
    }

    _i = 0;
    for (int i = 0; i < factor; i++) {
        _j = 1;
        for (int j = n - factor; j < n; j++) {
            temp->at<Vec3f>(i, j)[0] = temp->at<Vec3f>(_i, factor - _j)[0];
            temp->at<Vec3f>(i, j)[1] = temp->at<Vec3f>(_i, factor - _j)[1];
            temp->at<Vec3f>(i, j)[2] = temp->at<Vec3f>(_i, factor - _j)[2];
            _j++;
        }
        _i++;
    }

    _i = 1;
    for (int i = m - factor; i < m; i++) {
        _j = 0;
        for (int j = 0; j < factor; j++) {
            temp->at<Vec3f>(i, j)[0] = temp->at<Vec3f>(factor - _i, _j)[0];
            temp->at<Vec3f>(i, j)[1] = temp->at<Vec3f>(factor - _i, _j)[1];
            temp->at<Vec3f>(i, j)[2] = temp->at<Vec3f>(factor - _i, _j)[2];
            _j++;
        }
        _i++;
    }

    _i = 1;
    for (int i = m - factor; i < m; i++) {
        _j = 1;
        for (int j = n - factor; j < n; j++) {
            temp->at<Vec3f>(i, j)[0] = temp->at<Vec3f>(factor - _i, factor - _j)[0];
            temp->at<Vec3f>(i, j)[1] = temp->at<Vec3f>(factor - _i, factor - _j)[1];
            temp->at<Vec3f>(i, j)[2] = temp->at<Vec3f>(factor - _i, factor - _j)[2];
            _j++;
        }
        _i++;
    }
    //	print(temp);
    delete[] weight;
    return temp;

}

void Poisson::subtract(int num, Mat* mat)
{
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->at<Vec3f>(i, j)[0] = num - mat->at<Vec3f>(i, j)[0];
            mat->at<Vec3f>(i, j)[1] = num - mat->at<Vec3f>(i, j)[1];
            mat->at<Vec3f>(i, j)[2] = num - mat->at<Vec3f>(i, j)[2];
        }
    }
}

void Poisson::set(Mat* src, Mat* add, int times, int x, int y, int w, int h, float mi, float ma, int fa)
{
    init();
    isMask = false;
    srcImg = new Mat();
    addImg = new Mat();
    *srcImg = src->clone();
    *addImg = add->clone();
    iterTimes = times;
    beginw = x;
    beginh = y;
    width = w;
    height = h;
    min = mi;
    max = ma;
    factor = fa;
}

void Poisson::set(Mat* src, Mat* mask, Color c, int times, int x, int y, int w, int h)
{
    init();
    isMask = true;
    srcImg = new Mat();
    maskImg = new Mat();
    *srcImg = src->clone();
    *maskImg = mask->clone();
    iterTimes = times;
    color = c;
    beginw = x;
    beginh = y;
    width = w;
    height = h;
}

void Poisson::set(Mat* src, Color c, int times, int x, int y, int w, int h)
{
    init();
    isMask = false;
    srcImg = new Mat();
    color = c;
    *srcImg = src->clone();
    iterTimes = times;
    beginw = x;
    beginh = y;
    width = w;
    height = h;
}

void Poisson::set(Mat* src, Mat* add, Mat* mask, int times, int x, int y, int w, int h, float mi, float ma, int fa)
{
    init();
    isMask = true;
    srcImg = new Mat();
    addImg = new Mat();
    maskImg = new Mat();
    *srcImg = src->clone();
    *addImg = add->clone();
    *maskImg = mask->clone();
    iterTimes = times;
    beginw = x;
    beginh = y;
    width = w;
    height = h;
    min = mi;
    max = ma;
    factor = fa;
    qDebug()<<"set";
}

void Poisson::set(Mat* src, float a, float b, int times, int x, int y, int w, int h)
{
    init();
    isMask = false;

    srcImg = new Mat();
    *srcImg = src->clone();
    alpha = a;
    beta = b;
    iterTimes = times;
    beginw = x;
    beginh = y;
    width = w;
    height = h;
    magnitude = new Mat(Size(width, height), CV_32FC3);
}

void Poisson::set(Mat* src, Mat* mask, float a, float b, int times, int x, int y, int w, int h)
{
    init();
    isMask = true;
    srcImg = new Mat();
    maskImg = new Mat();

    *srcImg = src->clone();
    *maskImg = mask->clone();
    alpha = a;
    beta = b;
    iterTimes = times;
    beginw = x;
    beginh = y;
    width = w;
    height = h;

    magnitude = new Mat(Size(width, height), CV_32FC3);
}

void Poisson::set(Mat* src, Mat* mask, float lt, float ht, int size, int times, int x, int y, int w, int h)
{
    init();
    isMask = true;
    srcImg = new Mat();
    maskImg = new Mat();
    *srcImg = src->clone();
    *maskImg = mask->clone();
    iterTimes = times;
    beginw = x;
    beginh = y;
    width = w;
    height = h;
    low = lt;
    high = ht;
    ksize = size;
}

void Poisson::calculate(int i, int j)
{
    if (isMask && maskImg->at<float>(i, j) == 0) {
        return;
    }
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

void Poisson::run_normal()
{
    cal_gradient(addImg);

    tmp = (*srcImg)(Rect(beginw, beginh, width, height));
    solve_poisson1();
}


void Poisson::cal_light(float& g, float m)
{
    g = g * pow(alpha, beta) *pow(m, -1 * beta);
}

void Poisson::multi_color()
{
    for (int i = 0; i < gradient->rows; i++) {
        for (int j = 0; j < gradient->cols; j++) {
            if (isMask && maskImg->at<float>(i, j) == 0) {
                continue;
            }
            gradient->at<Vec3f>(i, j)[0] *= color.b;
            gradient->at<Vec3f>(i, j)[1] *= color.g;
            gradient->at<Vec3f>(i, j)[2] *= color.r;
        }
    }
}

void Poisson::multi_light()
{
    //print(gradientX);
    //print(magnitude);
    for (int i = 1; i < gradient->rows - 1; i++) {
        for (int j = 1; j < gradient->cols - 1; j++) {
            if (isMask && maskImg->at<float>(i, j) == 0) {
                continue;
            }
            cal_light(gradientX->at<Vec3f>(i, j)[0], magnitude->at<Vec3f>(i, j)[0]);
            cal_light(gradientX->at<Vec3f>(i, j)[1], magnitude->at<Vec3f>(i, j)[1]);
            cal_light(gradientX->at<Vec3f>(i, j)[2], magnitude->at<Vec3f>(i, j)[2]);
            cal_light(gradientY->at<Vec3f>(i, j)[0], magnitude->at<Vec3f>(i, j)[0]);
            cal_light(gradientY->at<Vec3f>(i, j)[1], magnitude->at<Vec3f>(i, j)[1]);
            cal_light(gradientY->at<Vec3f>(i, j)[2], magnitude->at<Vec3f>(i, j)[2]);
        }
    }

    patchNaNs(*gradientX);
    patchNaNs(*gradientY);

}

void Poisson::run_color()
{
    tmp = (*srcImg)(Rect(beginw, beginh, width, height));

    cal_gradient(&tmp);

    multi_color();

    solve_poisson1();
}

void Poisson::run_texture()
{
    tmp = (*srcImg)(Rect(beginw, beginh, width, height));

    gradientX = new Mat(Size(width, height), CV_32FC3);
    gradientY = new Mat(Size(width, height), CV_32FC3);
    cal_gradient(&tmp);
    cal_gradientX(&tmp, gradientX);
    cal_gradientY(&tmp, gradientY);

    Mat ctmp = tmp.clone();
    cvtColor(ctmp, ctmp, CV_BGR2GRAY);  //canny只处理灰度图
    ctmp.convertTo(ctmp, CV_8UC1, 255);
    Mat out = Mat(tmp.size(), CV_8UC1);
    Canny(ctmp, out, low, high, ksize);
    imshow("showcanny", out);
    canny_texture(out);

    imshow("tmp", tmp);

    solve_poisson2();
}

void Poisson::canny_texture(Mat out)
{
    Mat zeros( gradient->size(), CV_32FC3);
    zeros.setTo(0);
    Mat zerosMask = ((out) != 255);
    imshow("zerosMask",zerosMask);

    for(int i = 0;i < gradient->rows;i++) {
        for(int j = 0;j < gradient->cols;j++) {
            if(isMask && maskImg->at<float>(i,j)==0) {
                continue;
            }
            else{
                if(zerosMask.at<float>(i,j)!=0){
                    gradientX->at<Vec3f>(i,j)[0] *= 0;
                    gradientX->at<Vec3f>(i,j)[1] *= 0;
                    gradientX->at<Vec3f>(i,j)[2] *= 0;
                    gradientY->at<Vec3f>(i,j)[0] *= 0;
                    gradientY->at<Vec3f>(i,j)[1] *= 0;
                    gradientY->at<Vec3f>(i,j)[2] *= 0;
                }
            }
        }
    }

    Mat* newgradientX = new Mat(Size(width,height), CV_32FC3);
    Mat* newgradientY = new Mat(Size(width,height), CV_32FC3);

    cal_LaplacianX(gradientX,newgradientX);
    cal_LaplacianY(gradientY,newgradientY);
    *gradient = (*newgradientX) + (*newgradientY);

    delete newgradientX;
    delete newgradientY;
}

void Poisson::solve_poisson1()
{
    for (int k = 0; k <iterTimes; k++) {
        for (int i = 1; i < tmp.rows - 1; i++) {
            for (int j = 1; j < tmp.cols - 1; j++) {
                calculate(i, j);
            }
        }
    }
}

void Poisson::solve_poisson2()
{
    for (int k = 0; k <iterTimes; k++) {
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

float Poisson::pow2(float x)
{
    return x*x;
}

void Poisson::cal_magnitude(int i, int j, int k)
{
    magnitude->at<Vec3f>(i, j)[k] = sqrt(pow2(gradientX->at<Vec3f>(i, j)[k]) + pow2(gradientY->at<Vec3f>(i, j)[k]));
}

void Poisson::cal_magnitude()
{
    for (int i = 1; i < magnitude->rows - 1; i++) {
        for (int j = 1; j < magnitude->cols - 1; j++) {
            if (isMask && maskImg->at<float>(i, j) == 0) {
                continue;
            }
            cal_magnitude(i, j, 0);
            cal_magnitude(i, j, 1);
            cal_magnitude(i, j, 2);
        }
    }
}

void Poisson::run_light()
{
    Mat *newgradientX = new Mat(Size(width, height), CV_32FC3);
    Mat *newgradientY = new Mat(Size(width, height), CV_32FC3);
    gradientX = new Mat(Size(width, height), CV_32FC3);
    gradientY = new Mat(Size(width, height), CV_32FC3);
    tmp = (*srcImg)(Rect(beginw, beginh, width, height));

    cal_gradientX(&tmp, gradientX);
    cal_gradientY(&tmp, gradientY);

    cal_magnitude();
    multi_light();

    cal_gradientX(gradientX, newgradientX);
    cal_gradientY(gradientY, newgradientY);
    *gradient = *newgradientX + *newgradientY;

    solve_poisson1();
    delete newgradientX;
    delete newgradientY;
}

void Poisson::run_gradient()
{
    cal_gradient(addImg);
    Mat* alpha = mkTempCos(addImg->rows, addImg->cols);

    subtract(1, alpha);

    if (!isMask) {
        tmp = (*srcImg)(Rect(beginw, beginh, width, height));
    }
    else {
        tmp = (*srcImg)(Rect(beginw, beginh, width, height));
    }
    Mat origin = tmp.clone();
    solve_poisson2();

    for (int i = 0; i < tmp.rows; i++) {
        for (int j = 0; j < tmp.cols; j++) {
            tmp.at<Vec3f>(i, j)[0] = alpha->at<Vec3f>(i, j)[0] * tmp.at<Vec3f>(i, j)[0]
                + (1 - alpha->at<Vec3f>(i, j)[0]) * origin.at<Vec3f>(i, j)[0];
            tmp.at<Vec3f>(i, j)[1] = alpha->at<Vec3f>(i, j)[1] * tmp.at<Vec3f>(i, j)[1]
                + (1 - alpha->at<Vec3f>(i, j)[1]) * origin.at<Vec3f>(i, j)[1];
            tmp.at<Vec3f>(i, j)[2] = alpha->at<Vec3f>(i, j)[2] * tmp.at<Vec3f>(i, j)[2]
                + (1 - alpha->at<Vec3f>(i, j)[2]) * origin.at<Vec3f>(i, j)[2];
        }
    }
    delete alpha;
}

void Poisson::cal_gradient(Mat* img)
{
    for (int i = 1; i < img->rows - 1; i++) {
        for (int j = 1; j < img->cols - 1; j++) {
            if (isMask && maskImg->at<float>(i, j) == 0) {
                continue;
            }
            if (img->channels() == 3){
                gradient->at<Vec3f>(i, j)[0] = img->at<Vec3f>(i - 1, j)[0]
                    + img->at<Vec3f>(i + 1, j)[0]
                    + img->at<Vec3f>(i, j - 1)[0]
                    + img->at<Vec3f>(i, j + 1)[0]
                    - 4 * img->at<Vec3f>(i, j)[0];
                gradient->at<Vec3f>(i, j)[1] = img->at<Vec3f>(i - 1, j)[1]
                    + img->at<Vec3f>(i + 1, j)[1]
                    + img->at<Vec3f>(i, j - 1)[1]
                    + img->at<Vec3f>(i, j + 1)[1]
                    - 4 * img->at<Vec3f>(i, j)[1];
                gradient->at<Vec3f>(i, j)[2] = img->at<Vec3f>(i - 1, j)[2]
                    + img->at<Vec3f>(i + 1, j)[2]
                    + img->at<Vec3f>(i, j - 1)[2]
                    + img->at<Vec3f>(i, j + 1)[2]
                    - 4 * img->at<Vec3f>(i, j)[2];
            }
            else if (img->channels() == 1){
                gradient->at<Vec3f>(i, j)[0] = img->at<float>(i - 1, j)
                    + img->at<float>(i + 1, j)
                    + img->at<float>(i, j - 1)
                    + img->at<float>(i, j + 1)
                    - 4 * img->at<float>(i, j);
                gradient->at<Vec3f>(i, j)[1] = gradient->at<Vec3f>(i, j)[0];
                gradient->at<Vec3f>(i, j)[2] = gradient->at<Vec3f>(i, j)[0];
            }
        }
    }
}

void Poisson::cal_gradientX(Mat* img, Mat* gradientX)
{
    for (int i = 1; i < img->rows - 1; i++) {
        for (int j = 1; j < img->cols - 1; j++) {
            if (isMask && maskImg->at<float>(i, j) == 0){
                continue;
            }
            else {
                gradientX->at<Vec3f>(i, j)[0] = 0 * img->at<Vec3f>(i, j - 1)[0]
                    + 1 * img->at<Vec3f>(i, j + 1)[0]
                    - 1 * img->at<Vec3f>(i, j)[0];
                gradientX->at<Vec3f>(i, j)[1] = 0 * img->at<Vec3f>(i, j - 1)[1]
                    + 1 * img->at<Vec3f>(i, j + 1)[1]
                    - 1 * img->at<Vec3f>(i, j)[1];
                gradientX->at<Vec3f>(i, j)[2] = 0 * img->at<Vec3f>(i, j - 1)[2]
                    + 1 * img->at<Vec3f>(i, j + 1)[2]
                    - 1 * img->at<Vec3f>(i, j)[2];
            }
        }
    }
}

void Poisson::cal_gradientY(Mat* img, Mat* gradientY)
{
    for (int i = 1; i < img->rows - 1; i++) {
        for (int j = 1; j < img->cols - 1; j++) {
            if (isMask && maskImg->at<float>(i, j) == 0){
                continue;
            }
            else{
                gradientY->at<Vec3f>(i, j)[0] = 0 * img->at<Vec3f>(i - 1, j)[0]
                    + 1 * img->at<Vec3f>(i + 1, j)[0]
                    - 1 * img->at<Vec3f>(i, j)[0];
                gradientY->at<Vec3f>(i, j)[1] = 0 * img->at<Vec3f>(i - 1, j)[1]
                    + 1 * img->at<Vec3f>(i + 1, j)[1]
                    - 1 * img->at<Vec3f>(i, j)[1];
                gradientY->at<Vec3f>(i, j)[2] = 0 * img->at<Vec3f>(i - 1, j)[2]
                    + 1 * img->at<Vec3f>(i + 1, j)[2]
                    - 1 * img->at<Vec3f>(i, j)[2];
            }
        }
    }
}

void Poisson::cal_LaplacianX(Mat* img, Mat* gradientX)
{
    for (int i = 1; i < img->rows; i++) {
        for (int j = 1; j < img->cols; j++) {
            if (isMask && maskImg->at<float>(i, j) == 0){
                continue;
            }
            else
            {
                gradientX->at<Vec3f>(i, j)[0] = -1 * img->at<Vec3f>(i, j - 1)[0]
                    + 1 * img->at<Vec3f>(i, j)[0];
                gradientX->at<Vec3f>(i, j)[1] = -1 * img->at<Vec3f>(i, j - 1)[1]
                    + 1 * img->at<Vec3f>(i, j)[1];
                gradientX->at<Vec3f>(i, j)[2] = -1 * img->at<Vec3f>(i, j - 1)[2]
                    + 1 * img->at<Vec3f>(i, j)[2];
            }
        }
    }
}

void Poisson::cal_LaplacianY(Mat* img, Mat* gradientY)
{
    for (int i = 1; i < img->rows; i++) {
        for (int j = 1; j < img->cols; j++) {
            if (isMask && maskImg->at<float>(i, j) == 0){
                continue;
            }
            else{
                gradientY->at<Vec3f>(i, j)[0] = -1 * img->at<Vec3f>(i - 1, j)[0]
                    + 1 * img->at<Vec3f>(i, j)[0];
                gradientY->at<Vec3f>(i, j)[1] = -1 * img->at<Vec3f>(i - 1, j)[1]
                    + 1 * img->at<Vec3f>(i, j)[1];
                gradientY->at<Vec3f>(i, j)[2] = -1 * img->at<Vec3f>(i - 1, j)[2]
                    + 1 * img->at<Vec3f>(i, j)[2];
            }
        }
    }
}

void Poisson::run_mixed()
{
    qDebug()<<"run_mix";
    srcgradient = new Mat(Size(width, height), CV_32FC3);
    srcgradientX = new Mat(Size(width, height), CV_32FC3);
    srcgradientY = new Mat(Size(width, height), CV_32FC3);
    gradientX = new Mat(Size(width, height), CV_32FC3);
    gradientY = new Mat(Size(width, height), CV_32FC3);
    qDebug()<<"run_mix1";
    tmp = (*srcImg)(Rect(beginw, beginh, width, height));
    cal_gradient(&tmp);
    gradient->copyTo(*srcgradient);
    cal_gradientX(&tmp, gradientX);
    gradientX->copyTo(*srcgradientX);
    cal_gradientY(&tmp, gradientY);
    gradientY->copyTo(*srcgradientY);
    cal_gradient(addImg);
    cal_gradientX(addImg, gradientX);
    cal_gradientY(addImg, gradientY);
    Mat *newgradientX = new Mat(Size(width, height), CV_32FC3);
    Mat *newgradientY = new Mat(Size(width, height), CV_32FC3);
    qDebug()<<"run_mix2";
    for (int i = 0; i < tmp.rows; i++){
        for (int j = 0; j < tmp.cols; j++){
            float f0 = gradientX->at<Vec3f>(i, j)[0] - gradientY->at<Vec3f>(i, j)[0];
            float f1 = gradientX->at<Vec3f>(i, j)[1] - gradientY->at<Vec3f>(i, j)[1];
            float f2 = gradientX->at<Vec3f>(i, j)[2] - gradientY->at<Vec3f>(i, j)[2];
            float g0 = srcgradientX->at<Vec3f>(i, j)[0] - srcgradientY->at<Vec3f>(i, j)[0];
            float g1 = srcgradientX->at<Vec3f>(i, j)[1] - srcgradientY->at<Vec3f>(i, j)[1];
            float g2 = srcgradientX->at<Vec3f>(i, j)[2] - srcgradientY->at<Vec3f>(i, j)[2];
            if (isMask && maskImg->at<float>(i, j) == 0){
                continue;
            }
            else{
                if (abs(f0) <= abs(g0))
                {
                    gradientX->at<Vec3f>(i, j)[0] = srcgradientX->at<Vec3f>(i, j)[0];
                    gradientY->at<Vec3f>(i, j)[0] = srcgradientY->at<Vec3f>(i, j)[0];
                }
                if (abs(f1) <= abs(g1))
                {
                    gradientX->at<Vec3f>(i, j)[1] = srcgradientX->at<Vec3f>(i, j)[1];
                    gradientY->at<Vec3f>(i, j)[1] = srcgradientY->at<Vec3f>(i, j)[1];
                }
                if (abs(f2) <= abs(g2))
                {
                    gradientX->at<Vec3f>(i, j)[2] = srcgradientX->at<Vec3f>(i, j)[2];
                    gradientY->at<Vec3f>(i, j)[2] = srcgradientY->at<Vec3f>(i, j)[2];
                }
            }
        }
    }
    cal_LaplacianX(gradientX, newgradientX);
    cal_LaplacianY(gradientY, newgradientY);
    *gradient = (*newgradientX) + (*newgradientY);
    qDebug()<<"run_mix3";
    solve_poisson2();
    delete newgradientX;
    delete newgradientY;
    qDebug()<<"run_mix4";
}

void Poisson::run_gray()
{
    Mat *GraySrc = new Mat();
    *GraySrc = addImg->clone();
    cvtColor(*addImg, *GraySrc, COLOR_BGR2GRAY);
    cal_gradient(GraySrc);

    tmp = (*srcImg)(Rect(beginw, beginh, width, height));
    solve_poisson1();

    delete GraySrc;
}

Mat* Poisson::run(Type type)
{
    srcImg->convertTo(*srcImg, CV_32F, 1.0f / 255);

    if (addImg != nullptr) {
        addImg->convertTo(*addImg, CV_32F, 1.0f / 255);
        resize(*addImg, *addImg, Size(width, height));
    }
    if (maskImg != nullptr) {
        maskImg->convertTo(*maskImg, CV_32F, 1.0f / 255);
        resize(*maskImg, *maskImg, Size(width, height));
    }


    gradient = new Mat(Size(width, height), CV_32FC3);

    if (type == NORMAL) {
        run_normal();
    }
    else if (type == GRADIENT) {
        run_gradient();
    }
    else if (type == TEXTURE) {
        run_texture();
    }
    else if (type == COLOR) {
        run_color();
    }
    else if (type == LIGHT) {
        run_light();
    }
    else if (type == GRAY){
        qDebug()<<"run_gray";
        run_gray();
    }
    else if (type == MIXED) {
        run_mixed();
    }
    if (maskImg != nullptr) {
        delete maskImg;
    }
    if (addImg != nullptr) {
        delete addImg;
    }
    if (magnitude != nullptr) {
        delete magnitude;
    }
    if (gradientX != nullptr) {
        delete gradientX;
    }
    if (gradientY != nullptr) {
        delete gradientY;
    }
    if (srcgradientX != nullptr) {
        delete srcgradientX;
    }
    if (srcgradientY != nullptr) {
        delete srcgradientY;
    }
    if (srcgradient != nullptr) {
        delete srcgradient;
    }
    delete gradient;
    return srcImg;
}
