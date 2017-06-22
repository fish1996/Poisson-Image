#include "labtransfer.h"

LABtransfer::LABtransfer()
{
    transfer = nullptr;
}

LABtransfer::~LABtransfer()
{
    if (transfer != nullptr) {
        delete transfer;
    }
}

void LABtransfer::Getimg(Mat *src, Mat *tar)
{
    Mat srctmp;
    Mat tartmp;
    srctmp = (*src).clone();
    tartmp = (*tar).clone();
    source = srctmp.clone();
    target = tartmp.clone();
    //srctmp.convertTo(source, CV_32FC3, 1.0f/255.0f);
    //tartmp.convertTo(target, CV_32FC3, 1.0f/255.0f);
}

Mat LABtransfer::OutPut()
{
    transfer = new Mat(target.size(),CV_32FC3);
    color_transfer();
    //trans.convertTo(*transfer,CV_8U, 255.0, 1/255.0);

    return trans;
}
//RGB转换到Lab空间
Mat LABtransfer::RGBToLab(Mat m)
{
    Mat_<Vec3f> I = m;
    for(int i=0; i<I.rows; i++)
    {
        for(int j=0; j<I.cols; j++){
            double L = 0.3811*I.at<Vec3f>(i,j)[0] + 0.5783*I.at<Vec3f>(i,j)[1] + 0.0402*I.at<Vec3f>(i,j)[2];
            double M = 0.1967*I.at<Vec3f>(i,j)[0] + 0.7244*I.at<Vec3f>(i,j)[1] + 0.0782*I.at<Vec3f>(i,j)[2];
            double S = 0.0241*I.at<Vec3f>(i,j)[0] + 0.1288*I.at<Vec3f>(i,j)[1] + 0.8444*I.at<Vec3f>(i,j)[2];

            if(L == 0) L = 1;
            if(M == 0) M = 1;
            if(S == 0) S = 1;
            L = log(L);
            M = log(M);
            S = log(S);

            I.at<Vec3f>(i,j)[0] = (L+M+S) / sqrt(3.0);
            I.at<Vec3f>(i,j)[1] = (L+M-2*S) / sqrt(6.0);
            I.at<Vec3f>(i,j)[2] = (L-M) / sqrt(2.0);
        }
    }

    return I;
}

//Lab转换到RGB空间
Mat LABtransfer::LabToRGB(Mat m)
{
    Mat_<Vec3f> I = m;

    for(int i=0; i<I.rows; i++)
    {
        for(int j=0; j<I.cols; j++){
            double L = I.at<Vec3f>(i,j)[0]/sqrt(3.0) + I.at<Vec3f>(i,j)[1]/sqrt(6.0) + I.at<Vec3f>(i,j)[2]/sqrt(2.0);
            double M = I.at<Vec3f>(i,j)[0]/sqrt(3.0) + I.at<Vec3f>(i,j)[1]/sqrt(6.0) - I.at<Vec3f>(i,j)[2]/sqrt(2.0);
            double S = I.at<Vec3f>(i,j)[0]/sqrt(3.0) - 2*I.at<Vec3f>(i,j)[1]/sqrt(6.0);

            L = exp(L);
            M = exp(M);
            S = exp(S);

            I.at<Vec3f>(i,j)[0] = 4.4679*L - 3.5873*M + 0.1193*S;
            I.at<Vec3f>(i,j)[1] = -1.2186*L + 2.3809*M - 0.1624*S;
            I.at<Vec3f>(i,j)[2] = 0.0497*L - 0.2439*M + 1.2045*S;
        }
    }

    return I;
}

Vector<double> LABtransfer::computeMeans(Mat m)
{
    double sum[3] = { 0,0,0 };
    int pixes = m.cols * m.rows;
    Vector<double> means;

    means.resize(3);
    Mat_<Vec3f> I = m;

    for(int i=0; i<I.rows; i++)
    {
        for(int j=0; j<I.cols; j++){
            for(int k = 0;k < 3;k++)
            {
                sum[k] += I.at<Vec3f>(i,j)[k];
            }
        }
    }

    for(int i = 0;i < 3;i++)
    {
        means[i] = sum[i] / pixes;
    }
    return means;
}

Vector<double> LABtransfer::computeVariances(Mat m, Vector<double> means)
{
    double sum[3] = { 0,0,0 };
    int pixes = m.cols * m.rows;
    Mat_<Vec3f> I = m;
    Vector<double> variances;
    variances.resize(3);

    for(int i=0; i<I.rows; i++)
    {
        for(int j=0; j<I.cols; j++){
            for(int channel = 0;channel < 3;channel++){
                sum[channel] += (I.at<Vec3f>(i,j)[channel] - means[channel])*(I.at<Vec3f>(i,j)[channel] - means[channel]); //有问题
            }
        }
    }
    for(int i = 0;i < 3;i++){
        variances[i] = sqrt(sum[i] / pixes);
    }
    return variances;
}

void LABtransfer::image_stats()
{
    sourcelab = RGBToLab(source);
    targetlab = RGBToLab(target);
    srcMeans = computeMeans(sourcelab);
    targetMeans = computeMeans(targetlab);
    srcVariances = computeVariances(sourcelab, srcMeans);
    targetVariances = computeVariances(targetlab, targetMeans);
    trans = source;
}

void LABtransfer::color_transfer()
{
    image_stats();

    Mat_<Vec3f> I = trans;

    double sum[3] = { 0 };
    for(int channel =0;channel < 3;channel++)
    {
        sum[channel] = targetVariances[channel] / srcVariances[channel];
    }

    for(int i=0; i<I.rows; i++)
    {
        for(int j=0; j<I.cols; j++){
            for(int channel = 0;channel < 3;channel++){
                I.at<Vec3f>(i,j)[channel] = sum[channel] * (I.at<Vec3f>(i,j)[channel]-srcMeans[channel]) + targetMeans[channel];
            }
        }
    }
    trans = LabToRGB(trans);
    cout<<trans<<endl;
}
