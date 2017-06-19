#include "paracontrol.h"

para::para()
{
    this->type = NORMAL;
    this->choosenum = 0;
    this->iteration = 1000;
    this->hscaling = 1;
    this->vscaling = 1;
    this->min = 5;
    this->max = 200;
    this->factor = 10;
    this->R = 0;
    this->G = 0;
    this->B = 0;
    this->ope = 3;
    this->alpha = 0.2;
    this->beta = 0.4;

    this->scr = new backwindow;
    this->dst = new backwindow;
    this->chooseimg = new ChooseImg;

    chooseimg->setParent(this->dst);
    chooseimg->show();
    chooseimg->raise();
    chooseimg->setVisible(false);
}

para::~para(){}

void para::updataPara(Type type, int choosenum, int iteration, float hscaling, float vscaling, int min, int max, int factor)
{
    this->type = type;
    this->choosenum = choosenum;
    this->iteration = iteration;
    this->hscaling = hscaling;
    this->vscaling = vscaling;
    this->min = min;
    this->max = max;
    this->factor = factor;

    return;
}

void para::updataType(Type type)
{
    this->type = type;

    return;
}

void para::updataChoose(int choosenum)
{
    this->choosenum = choosenum;
    ChangeChoose(choosenum);
    return;
}

void para::updataItera(int iteration)
{
    this->iteration = iteration;

    return;
}

void para::updataHscaling(int hsacling)
{
    this->hscaling = hsacling / 200.0;

    return;
}

void para::updataVscaling(int vscaling)
{
    this->vscaling = vscaling / 200.0;

    return;
}

void para::updataThreshold(int min, int max, int factor)
{
    this->min = min;
    this->max = max;
    this->factor = factor;

    return;
}

void para::PassMaskAndSrc()
{
    Mat add = scr->PutFitSrc();
    Mat mask = scr->PutFitMask();
    dst->GetMaskAndSrc(add, mask);
    chooseimg->GetImg(scr->PutFitSrc());
}

void para::startPossion(int times, Type type)
{
    this->scr->ChangeSize(chooseimg->sw, chooseimg->sh);
    this->PassMaskAndSrc();
    qDebug("w&h:%d,%d\n", dst->fitsrc.rows, dst->fitsrc.cols);
    this->dst->poisson = new Poisson();
    if(type == NORMAL){
        this->dst->poisson->set(&dst->srcImg, &dst->fitsrc, &dst->fitmask, times,
                                chooseimg->ImgPos.x(), chooseimg->ImgPos.y(), chooseimg->w, chooseimg->h);
    }
    else if(type == GRAY){
        qDebug()<<"gray";
        this->dst->poisson->set(&dst->srcImg, &dst->fitsrc, &dst->fitmask, times,
                                chooseimg->ImgPos.x(), chooseimg->ImgPos.y(), chooseimg->w, chooseimg->h);
    }
    else if(type == MIXED){
        qDebug()<<"MIXED";
        this->dst->poisson->set(&dst->srcImg, &dst->fitsrc, &dst->fitmask, times,
                                chooseimg->ImgPos.x(), chooseimg->ImgPos.y(), chooseimg->w, chooseimg->h);
    }
    else if(type == TEXTURE){
        this->dst->srcImg = this->scr->srcImg;
        this->dst->maskImg = this->scr->maskImg;
        this->dst->poisson->set(&dst->srcImg, &dst->maskImg, (float)min, (float)max, ope, times,
                                chooseimg->ImgPos.x(), chooseimg->ImgPos.y(), chooseimg->w, chooseimg->h);
    }
    else if(type == LIGHT){
        this->dst->srcImg = this->scr->srcImg;
        this->dst->maskImg = this->scr->maskImg;
        this->dst->poisson->set(&dst->srcImg, &dst->maskImg, alpha, beta, times,
                                chooseimg->ImgPos.x(), chooseimg->ImgPos.y(), chooseimg->w, chooseimg->h);
    }
    else if(type == COLOR){
        this->dst->srcImg = this->scr->srcImg;
        this->dst->maskImg = this->scr->maskImg;
        this->dst->poisson->set(&dst->srcImg, &dst->maskImg, Color(R, G, B), times,
                                chooseimg->ImgPos.x(), chooseimg->ImgPos.y(), chooseimg->w, chooseimg->h);
    }
    else if(type == OPTIMISED){
        qDebug()<<"OPTIMISED";
        this->dst->poisson->set(&dst->srcImg, &dst->fitsrc, &dst->fitmask, times,
                                chooseimg->ImgPos.x(), chooseimg->ImgPos.y(), chooseimg->w, chooseimg->h);
    }
    else if(type == SPLICING){
        //
    }
    else
        return;
    //this->dst->poisson->set(dst->srcImg, dst->fitsrc, dst->fitmask, times, x, y, w, h);
    this->dst->start(type);
}

Type para::GetType()
{
    return this->type;
}

int para::GetChoose()
{
    return this->choosenum;
}

int para::GetIteration()
{
    return this->iteration;
}

float para::GetHscaling()
{
    return this->hscaling;
}

float para::GetVscaling()
{
    return this->vscaling;
}

void para::ChangeChoose(int choosenum)
{
    scr->GetChoose(this->choosenum);
}
