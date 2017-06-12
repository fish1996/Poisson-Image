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
}

backwindow::~backwindow()
{
    timer->stop();
    delete timer;
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

void backwindow::clear()
{
    inum = -1;
    co_ordinates.clear();
    maskImg.release();

    update();
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
        if(flag){
            //dstImg = srcImg.clone();
            maskImg = Mat::zeros(srcImg.rows,srcImg.cols, CV_8UC1);
            for(int i=0;i<=inum;i++){
                drawContours(maskImg,co_ordinates, i, Scalar(255), CV_FILLED, 8 );
            }
            qDebug()<<maskImg.cols<<maskImg.rows;
            imshow("a",maskImg);
        }
    }

    else if(e->button() == Qt::LeftButton)
    {
        if(flag){
            inum++;
            flags=false;

            QPoint p=e->pos(); //获取鼠标点击处点

            if(flag){ //获取初始形状
                if(p.x()<width && p.y()<height){
                    pt_origin = Point(e->x(),e->y());
                    co_ordinates.push_back(vector<Point>());
                    co_ordinates[inum].push_back(pt_origin);
                }
            }
        }
    }

    update();
}

void backwindow::mouseMoveEvent(QMouseEvent *e)
{
    if(flag && !flags)
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

            /*qDebug()<<"flag"<<flag;
            qDebug()<<p;
            qDebug()<<inum;*/
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

void backwindow::start(){
    Mat src = imread("1.jpg");
    qDebug()<<"sourcesize = "<<src.rows << " "<<src.cols;
    qDebug()<<"masksize = "<<maskImg.rows << " "<<maskImg.cols;
    poisson = new Poisson();
    poisson->set(&src,&srcImg,&maskImg,3000,100, 100, 100,60);
    Mat* ans = poisson->run(Type::NORMAL);
    imshow("miao",*ans);
}
