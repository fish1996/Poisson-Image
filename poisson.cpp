
#include "poisson.h"
#include <fstream>
Poisson::Poisson()
{
    PI = 3.14159;
    srcImg = nullptr;
    addImg = nullptr;
    maskImg = nullptr;
    //out.open("test.txt");
    //freopen("test.txt", "w", stdout);
}

void Poisson::print(Mat* temp)
{
    printf("%d %d\n", temp->rows, temp->cols);
    for (int i = 0; i < temp->rows; i++){
        printf("row %d: ", i+1);
        for (int j = 0; j < temp->cols; j++){
            printf("%.6f ",temp->at<Vec3f>(i, j)[0]);
        }
        printf("\n");
    }
}

Mat* Poisson::mkTempCos(int m, int n)
{
    int _i = 0, _j = 0;
    float* weight = new float[factor];
    Mat* temp = new Mat(Size(n,m), CV_32FC3, Scalar(0));
    for (int i = 0; i < factor; i++) {
        weight[i] = 1.0*(max - min)*(cos(PI*i / factor) + 1)/2 + min;
    }

    for (int i = factor; i < m - factor; i++){
        _j = 0;
        for (int j = 0; j < factor; j++){
            temp->at<Vec3f>(i, j)[0] = weight[_j];
            temp->at<Vec3f>(i, j)[1] = weight[_j];
            temp->at<Vec3f>(i, j)[2] = weight[_j];
            _j++;
        }
    }

    for (int i = factor; i < m - factor; i++){
        _j = 1;
        for (int j = n - factor; j < n; j++){
            temp->at<Vec3f>(i, j)[0] = weight[factor - _j];
            temp->at<Vec3f>(i, j)[1] = weight[factor - _j];
            temp->at<Vec3f>(i, j)[2] = weight[factor - _j];
            _j++;
        }
    }

    _i = 0;
    for (int i = 0; i < factor; i++){
        for (int j = factor; j < n - factor; j++){
            temp->at<Vec3f>(i, j)[0] = weight[_i];
            temp->at<Vec3f>(i, j)[1] = weight[_i];
            temp->at<Vec3f>(i, j)[2] = weight[_i];
        }
        _i++;
    }

    _i = 1;
    for (int i = m - factor; i < m; i++){
        for (int j = factor; j < n - factor; j++){
            temp->at<Vec3f>(i, j)[0] = weight[factor - _i];
            temp->at<Vec3f>(i, j)[1] = weight[factor - _i];
            temp->at<Vec3f>(i, j)[2] = weight[factor - _i];
        }
        _i++;
    }

    for (int i = 0; i < factor; i++){
        for (int j = 0; j < factor; j++){
            if (i > j){
                temp->at<Vec3f>(i, j)[0] = weight[j];
                temp->at<Vec3f>(i, j)[1] = weight[j];
                temp->at<Vec3f>(i, j)[2] = weight[j];
            }
            else{
                temp->at<Vec3f>(i, j)[0] = weight[i];
                temp->at<Vec3f>(i, j)[1] = weight[i];
                temp->at<Vec3f>(i, j)[2] = weight[i];
            }
        }
    }

    _i = 0;
    for (int i = 0; i < factor; i++){
        _j = 1;
        for (int j = n - factor; j < n; j++){
            temp->at<Vec3f>(i, j)[0] = temp->at<Vec3f>(_i, factor - _j)[0];
            temp->at<Vec3f>(i, j)[1] = temp->at<Vec3f>(_i, factor - _j)[1];
            temp->at<Vec3f>(i, j)[2] = temp->at<Vec3f>(_i, factor - _j)[2];
            _j++;
        }
        _i++;
    }

    _i = 1;
    for (int i = m - factor; i < m; i++){
        _j = 0;
        for (int j = 0; j < factor; j++){
            temp->at<Vec3f>(i, j)[0] = temp->at<Vec3f>(factor - _i, _j)[0];
            temp->at<Vec3f>(i, j)[1] = temp->at<Vec3f>(factor - _i, _j)[1];
            temp->at<Vec3f>(i, j)[2] = temp->at<Vec3f>(factor - _i, _j)[2];
            _j++;
        }
        _i++;
    }

    _i = 1;
    for (int i = m - factor; i < m; i++){
        _j = 1;
        for (int j = n - factor; j < n; j++){
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
    for (int i = 0; i < mat->rows; i++){
        for (int j = 0; j < mat->cols; j++){
            mat->at<Vec3f>(i, j)[0] = num - mat->at<Vec3f>(i, j)[0];
            mat->at<Vec3f>(i, j)[1] = num - mat->at<Vec3f>(i, j)[1];
            mat->at<Vec3f>(i, j)[2] = num - mat->at<Vec3f>(i, j)[2];
        }
    }
}

void Poisson::set(Mat* src, Mat* add, int times, int x, int y, int w, int h, float mi, float ma, int fa)
{
    isMask = false;
    srcImg = new Mat();
    addImg = new Mat();
    *srcImg = src->clone();
    *addImg = add->clone();
    iterTimes = times;
    begin = x;
    end = y;
    width = w;
    height = h;
    min = mi;
    max = ma;
    factor = fa;
}

void Poisson::set(Mat* src, Mat* add, Mat* mask,int times, int x, int y, int w, int h, float mi, float ma, int fa)
{
    isMask = false;
    srcImg = new Mat();
    addImg = new Mat();
    maskImg = new Mat();
    *srcImg = src->clone();
    *addImg = add->clone();
    *maskImg = mask->clone();
    iterTimes = times;
    begin = x;
    end = y;
    width = w;
    height = h;
    min = mi;
    max = ma;
    factor = fa;
}

void Poisson::calculate(int i,int j)
{
    if(isMask && maskImg->at<float>(i,j)==0) {
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

void Poisson::run_texture()
{
    cal_gradient();

    tmp = (*srcImg)(Rect(begin, end, width, height));
    for (int k = 0; k <iterTimes; k++){
        for (int i = 1; i < tmp.rows - 1; i++){
            for (int j = 1; j < tmp.cols - 1; j++){
                calculate(i, j);
            }
        }
    }
}


void Poisson::run_normal()
{
    cal_gradient();

    if(!isMask) {
        tmp = (*srcImg)(Rect(begin, end, width, height));
    }
    else if(isMask) {

    }
    for (int k = 0; k <iterTimes; k++){
        for (int i = 1; i < tmp.rows - 1; i++){
            for (int j = 1; j < tmp.cols - 1; j++){
                calculate(i, j);
            }
        }
    }
}

void Poisson::run_color()
{
    cal_gradient();
}

void Poisson::run_gradient()
{
    cal_gradient();

    Mat* alpha = mkTempCos(addImg->rows, addImg->cols);

    subtract(1, alpha);

    if(!isMask) {
        tmp = (*srcImg)(Rect(begin, end, width, height));
    }
    else {
        tmp = (*srcImg)(Rect(begin, end, width, height));
    }
    Mat origin = tmp.clone();

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

    for (int i = 0; i < tmp.rows; i++){
        for (int j = 0; j < tmp.cols; j++){
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

void Poisson::cal_gradient()
{
    for (int i = 1; i < addImg->rows - 1; i++) {
        for (int j = 1; j < addImg->cols - 1; j++) {
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
}

Mat* Poisson::run(Type type)
{
    srcImg->convertTo(*srcImg, CV_32F, 1.0f / 255);
    addImg->convertTo(*addImg, CV_32F, 1.0f / 255);
    if(maskImg!=nullptr) {
        maskImg->convertTo(*maskImg, CV_32F, 1.0f / 255);
        resize(*maskImg, *maskImg, Size(width, height));
    }
    resize(*addImg, *addImg, Size(width, height));

    gradient = new Mat(addImg->size(), CV_32FC3);

    if (type == Type::NORMAL){
        run_normal();
    }
    else if (type == Type::GRADIENT){
        run_gradient();
    }
    else if (type == Type::TEXTURE) {
        run_texture();
    }
    else if (type == Type::COLOR) {
        run_color();
    }
    if(maskImg != nullptr) {
        delete maskImg;
    }
    delete addImg;
    delete gradient;
    return srcImg;
}
