#include "backwindow.h"

backwindow::backwindow(QWidget* parent):QWidget(parent)
{
    //this->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);//设置自定义无边框窗口；
    //this->show(); //设置后重新显示；
    flag = false;
    flags = true;
    inum = -1;

    pt_origin = Point(-1,-1);
    pt_Cur = Point(-1,-1);
    co_ordinates.clear();

    timer = new QTimer(this); //新建定时器
    if(timer==NULL)
    {
        qDebug()<<"fail timer!";
    }
    else{
        qDebug()<<"success timer!"; //确定计时器已建立
    }
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    if(!timer->isActive()){
        timer->start(1000);
        qDebug()<<"timer start!";
    }

    fitw = 0;
    fith = 0;
    choosenum = 0;
    alphaimg = nullptr;
}

backwindow::~backwindow()
{
    timer->stop();
    delete timer;
    delete alphaimg;
}

void backwindow::open(QString filename)
{
    image = imread(filename.toStdString());
    cvtColor(image,image,CV_BGR2RGB); //调整opencv中mat颜色通道bgr至Qimage中rgb
    image = resizemat(image);
    QImage img = QImage((const unsigned char*)(image.data),image.cols,image.rows,image.cols*image.channels(),QImage::Format_RGB888);
    //pix = QPixmap::fromImage( img.scaled(size(), Qt::KeepAspectRatio) );
    pix = QPixmap::fromImage(img);

    flag=true;

    srcImg = image.clone();
    cvtColor(srcImg,srcImg,CV_RGB2BGR);
    w = image.cols;
    h = image.rows;

    update();
}

Mat backwindow::resizemat(Mat img)
{
    int w = img.cols;
    int h = img.rows;

    Mat dst;

    if(w >= width && h >= height) //图像大于画布
    {
        if(w*height>=h*width){ //宽度差距更大

            cv::resize(img, dst, Size(), (width*1.0/w), (width*1.0/w));
        }
        else{
            cv::resize(img, dst, Size(), (height*1.0/h), (height*1.0/h));
        }
    }
    else if((w < width && h < height))
    {
        if(w*height>=h*width){ //宽度差距更小
            cv::resize(img, dst, Size(), (width*1.0/w), (width*1.0/w));
        }
        else{
            cv::resize(img, dst, Size(), (height*1.0/h), (height*1.0/h));
        }

    }
    else if(w >= width && h < height)
    {
        cv::resize(img, dst, Size(), (width*1.0/w), (width*1.0/w));
    }
    else
    {
        cv::resize(img, dst, Size(), (height*1.0/h), (height*1.0/h));
    }
    return dst;
}

void backwindow::GetMask()
{
    if(flag){
        //dstImg = srcImg.clone();
        maskImg = Mat::zeros(srcImg.rows,srcImg.cols, CV_8UC1);
        for(int i=0;i<=inum;i++){
            drawContours(maskImg,co_ordinates, i, Scalar(255), CV_FILLED, 8 );
        }
        imshow("a",maskImg);
    }

}

Mat backwindow::PutMask()
{
    return maskImg;
}

Mat backwindow::PutSrc()
{
    return srcImg;
}

Mat backwindow::PutFitSrc()
{
    return fitsrc;
}

Mat backwindow::PutFitALP()
{
    return *alphaimg;
}

Mat backwindow::PutFitMask()
{
    return fitmask;
}

void backwindow::GetMaskAndSrc(Mat src, Mat mask)
{
    fitsrc = src;
    fitmask = mask;
}

void backwindow::clear()
{
    inum = -1;
    co_ordinates.clear();
    maskImg.release();

    update();
}

void backwindow::finish()
{
    fitmask = maskImg.clone();
    fitsrc = srcImg.clone();

    int xmin,xmax,ymin,ymax;
    xmin=ymin=10000;
    xmax=ymax=0;
    for(int i=0;i<fitmask.rows;i++)
    {
        for(int j=0;j<fitmask.cols;j++)
        {
            if(fitmask.at<uchar>(i,j)==255)
            {
                if(xmin>j)
                {
                    xmin=j;
                }
                if(xmax<j)
                {
                    xmax=j;
                }
                if(ymin>i)
                {
                    ymin=i;
                }
                if(ymax<i)
                {
                    ymax=i;
                }

            }
        }
    }
    if(ymin==0) ymin=1;
    if(xmin==0) xmin=1;
    if(xmax==w-1) xmax=w-2;
    if(ymax==h-1) ymax=h-2;
    fitmask=fitmask(Range(ymin-1,ymax+2),Range(xmin-1,xmax+2));
    fitsrc=fitsrc(Range(ymin-1,ymax+2),Range(xmin-1,xmax+2));
    fitw = fitmask.cols;
    fith = fitmask.rows;
    imshow("mask",fitsrc);

    alphaimg = new Mat(fitsrc.size(),CV_8UC4);
    createAlphaMat(alphaimg);
}

void backwindow::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, pix);

    for(int i=0;i<=inum;i++)
    {
        painter.setPen(QPen(Qt::black,2, Qt::SolidLine, Qt::RoundCap)); //设置画笔特性，同于绘制点
        for(int j=0;j<co_ordinates[i].size();j++){
            painter.drawPoint(co_ordinates[i][j].x,co_ordinates[i][j].y);
        }

        painter.setPen(QPen(Qt::black,1, Qt::SolidLine, Qt::RoundCap)); //设置画笔特性，同于绘制点
        if(co_ordinates[i].size()>1)
        {
            for(int j=0;j<co_ordinates[i].size();j++){
                int x1=co_ordinates[i][j].x;
                int y1=co_ordinates[i][j].y;
                int x2=co_ordinates[i][j+1].x;
                int y2=co_ordinates[i][j+1].y;
                if(j==co_ordinates[i].size()-1){
                    x2=co_ordinates[i][0].x;
                    y2=co_ordinates[i][0].y;
                }
                painter.drawLine(x1,y1,x2,y2);
            }
        }
    }
}

void backwindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::RightButton)
    {
    }

    else if(e->button() == Qt::LeftButton)
    {
        if(flag){
            inum++;
            flags=false;

            QPoint p=e->pos(); //获取鼠标点击处点

            if(p.x()<width && p.y()<height){
                pt_origin = Point(e->x(),e->y());
                co_ordinates.push_back(vector<Point>());
                co_ordinates[inum].push_back(pt_origin);
            }
        }
    }

    update();
}

void backwindow::mouseMoveEvent(QMouseEvent *e)
{
    int startx = co_ordinates[inum][0].x;
    int starty = co_ordinates[inum][0].y;
    if(flag && !flags && choosenum == 1)
    {
        QPoint p=e->pos();
        if(p.x()<width && p.y()<height){
            pt_Cur = Point(p.x(),p.y());
            if(pt_Cur.x>w){
                pt_Cur.x=w;
            }
            if(pt_Cur.x<0){
                pt_Cur.x=0;
            }
            if(pt_Cur.y>h)
            {
                pt_Cur.y=h;
            }
            if(pt_Cur.y<0){
                pt_Cur.y=0;
            }
            co_ordinates[inum].push_back(pt_Cur);

        }
    }
    else if(flag && !flags && choosenum == 0)
    {
        QPoint p=e->pos();
        if(p.x()<width && p.y()<height){
            pt_Cur = Point(p.x(),p.y());
            if(pt_Cur.x>w){
                pt_Cur.x=w;
            }
            if(pt_Cur.x<0){
                pt_Cur.x=0;
            }
            if(pt_Cur.y>h)
            {
                pt_Cur.y=h;
            }
            if(pt_Cur.y<0){
                pt_Cur.y=0;
            }
            co_ordinates[inum].clear();

            co_ordinates[inum].push_back(Point(startx,starty));
            co_ordinates[inum].push_back(Point(startx,pt_Cur.y));
            co_ordinates[inum].push_back(Point(pt_Cur.x,pt_Cur.y));
            co_ordinates[inum].push_back(Point(pt_Cur.x,starty));
        }
    }
    update();
}

void backwindow::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        flags = true;
    }
}

void backwindow::GetChoose(int choose)
{
    choosenum = choose;
}

void backwindow::ChangeSize(int sw, int sh)
{
    cv::resize(fitmask, fitmask, Size(), (sw*1.0/fitw), (sh*1.0/fith));
    cv::resize(fitsrc, fitsrc, Size(), (sw*1.0/fitw), (sh*1.0/fith));
}

void backwindow::createAlphaMat(Mat *png)
{
    for(int i=0;i<fitmask.rows;i++)
    {
        for(int j=0;j<fitmask.cols;j++)
        {
            Vec4b& rgba = png->at<Vec4b>(i, j);
            rgba[0] = fitsrc.at<Vec3b>(i,j)[0];
            rgba[1] = fitsrc.at<Vec3b>(i,j)[1];
            rgba[2] = fitsrc.at<Vec3b>(i,j)[2];
            rgba[3] = fitmask.at<uchar>(i,j);
        }
    }
}

void backwindow::start(Type type){
    /*Mat src = imread("C:/Users/ThinkPad/Desktop/build-poisson-Desktop_Qt_5_7_0_MSVC2013_64bit-Release/release/1.jpg");
    qDebug()<<"sourcesize = "<<src.rows << " "<<src.cols;
    qDebug()<<"masksize = "<<maskImg.rows << " "<<maskImg.cols;
    poisson = new Poisson();
    poisson->set(&src,&srcImg,&maskImg,3000,100, 100, 100,60);
    Mat* ans = poisson->run(Type::NORMAL);
    imshow("miao",*ans);*/
    /*poisson = new Poisson();
    poisson->set(&srcImg, &fitsrc, &fitmask, times, x, y, w, h);*/
    imshow("1",fitsrc);
    //imshow("2",fitmask);
    //imshow("3",srcImg);
    qDebug()<<"fit";
    Mat *ans = poisson->run(type);
    imshow("miao",*ans);
    //srcImg = *ans;
}
