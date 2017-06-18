#include "chooseimg.h"

ChooseImg::ChooseImg():QLabel()
{
    IsMove = false;
    CanMove = false;
    ImgPos = QPoint(0, 0);
    old = QPoint(0, 0);
}

void ChooseImg::CanMoveImg()
{
    CanMove = !CanMove;
}

void ChooseImg::ChangeW(float cw)
{
    this->resize(w*cw, h);
}

void ChooseImg::ChangeH(float ch)
{
    this->resize(w, h*ch);
}

void ChooseImg::GetImg(Mat fitImg)
{
    Mat rgb;
    QImage img;
    if(fitImg.channels() == 3)    // RGB image
    {
        cvtColor(fitImg,rgb,CV_BGR2RGB);
        img = QImage((const uchar*)(rgb.data),  //(const unsigned char*)
                     rgb.cols,rgb.rows,
                     rgb.cols*rgb.channels(),   //new add
                     QImage::Format_RGB888);
    }else                     // gray image
    {
        img = QImage((const uchar*)(fitImg.data),
                     fitImg.cols,fitImg.rows,
                     fitImg.cols*fitImg.channels(),    //new add
                     QImage::Format_Indexed8);
    }

    w = img.width();
    h = img.height();
    this->setPixmap(QPixmap::fromImage(img));
    this->resize(this->pixmap()->size());
    this->setGeometry(QRect(0, 0, this->pixmap()->size().width(), this->pixmap()->size().height()));
    this->setVisible(false);
    //chooseimg->setGeometry(QRect(0, 0, chooseimg->w, chooseimg->h));
}

void ChooseImg::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton && CanMove == true){
        old = e->globalPos();
        qDebug("%d,%d",ImgPos.x(),ImgPos.y());
        IsMove = true;
    }
}

void ChooseImg::mouseMoveEvent(QMouseEvent *e)
{
    if(IsMove == true)
    {
        QPoint newP = e->globalPos();
        int tmpx = ImgPos.x() + newP.x() - old.x();
        int tmpy = ImgPos.y() + newP.y() - old.y();
        //qDebug("%d,%d",ImgPos.x(),ImgPos.y());
        ImgPos = QPoint(tmpx, tmpy);
        if(ImgPos.x() < 0)
            ImgPos.setX(0);
        if(ImgPos.x() > 580 - this->width())
            ImgPos.setX(580 - this->width());
        if(ImgPos.y() < 0)
            ImgPos.setY(0);
        if(ImgPos.y() > 450 - this->height())
            ImgPos.setY(450 - this->height());
        this->move(ImgPos);
        old = newP;
    }
}

void ChooseImg::mouseReleaseEvent(QMouseEvent *e)
{
    qDebug("%d,%d",e->globalPos().x(),e->globalPos().y());
    IsMove = false;
}
