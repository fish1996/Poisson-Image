#include "backwindow.h"

backwindow::backwindow(QWidget* parent):QWidget(parent)
{
    //this->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);//设置自定义无边框窗口；
    //this->show(); //设置后重新显示；
    flag = false;
    flags = true;
    /*x = new int[10000]();
    y = new int[10000]();
    num = -1;*/
    inum = -1;

    pt_origin = Point(-1,-1);
    pt_Cur = Point(-1,-1);
    co_ordinates.clear();
}

backwindow::~backwindow()
{
}

void backwindow::open(QString filename)
{
    image = imread(filename.toStdString());
    cvtColor(image,image,CV_BGR2RGB); //调整opencv中mat颜色通道bgr至Qimage中rgb
    QImage img = QImage((const unsigned char*)(image.data),image.cols,image.rows,image.cols*image.channels(),QImage::Format_RGB888);
    pix = QPixmap::fromImage( img.scaled(size(), Qt::KeepAspectRatio) );

    flag=true;

    srcImg = image.clone();

    update();
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
    painter.drawPixmap(0, 0, 400, 400, pix);
    qDebug()<<"Begin painting";

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
    if(e->button() == Qt::LeftButton)
    {
        if(flag){
            inum++;
            flags=false;

            QPoint p=e->pos(); //获取鼠标点击处点

            if(flag){ //获取初始形状
                if(p.x()<width && p.y()<height){
                    /*num++; //点数量增加，num=点数-1；
                    x[num]=e->x();
                    y[num]=e->y(); //记录横纵坐标*/
                    pt_origin = Point(e->x(),e->y());
                    co_ordinates.push_back(vector<Point>());
                    co_ordinates[inum].push_back(pt_origin);
                }
            }
        }
    }
    if(e->button() == Qt::RightButton)
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

    update();
}

void backwindow::mouseMoveEvent(QMouseEvent *e)
{
    if(flag && !flags)
    {
        QPoint p=e->pos();
        if(p.x()<width && p.y()<height){
            pt_Cur = Point(p.x(),p.y());
            co_ordinates[inum].push_back(pt_Cur);

            qDebug()<<"flag"<<flag;
            qDebug()<<p;
            qDebug()<<inum;
        }
    }

    update();
}

void backwindow::start(){
    Mat src = imread("2.jpg");
    poisson = new Poisson();
    poisson->set(&src,&srcImg,&maskImg,3000,190, 150, 100,60);
}
