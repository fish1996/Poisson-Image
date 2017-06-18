#include "paracontrol.h"

para::para()
{
    this->oper = 0;
    this->choosenum = 0;
    this->iteration = 1000;
    this->hscaling = 1;
    this->vscaling = 1;
    this->min = 0;
    this->max = 1;
    this->factor = 10;

    this->scr = new backwindow;
    this->dst = new backwindow;
    this->chooseimg = new ChooseImg;

    chooseimg->setParent(this->dst);
    chooseimg->show();
    chooseimg->raise();
    chooseimg->setVisible(false);
}

para::~para(){}

void para::updataPara(int oper, int choosenum, int iteration, float hscaling, float vscaling, int min, int max, int factor)
{
    this->oper = oper;
    this->choosenum = choosenum;
    this->iteration = iteration;
    this->hscaling = hscaling;
    this->vscaling = vscaling;
    this->min = min;
    this->max = max;
    this->factor = factor;

    return;
}

void para::updataOper(int oper)
{
    this->oper = oper;

    return;
}

void para::updataChoose(int choosenum)
{
    this->choosenum = choosenum;

    return;
}

void para::updataItera(int iteration)
{
    this->iteration = iteration;

    return;
}

void para::updataHscaling(int hsacling)
{
    this->hscaling = hscaling / 200.0;

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
    this->PassMaskAndSrc();
    qDebug("w&h:%d,%d\n", chooseimg->w, chooseimg->h);
    this->dst->start(times, chooseimg->ImgPos.x(), chooseimg->ImgPos.y(), chooseimg->w, chooseimg->h,type);
}

int para::GetOper()
{
    return this->oper;
}

int para::GetChoose()
{
    return this->choosenum;
}

int para::GetIteration()
{
    return this->iteration;
}

int para::GetHscaling()
{
    return this->hscaling;
}

int para::GetVscaling()
{
    return this->vscaling;
}
