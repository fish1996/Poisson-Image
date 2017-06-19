#ifndef PARACONTROL_H
#define PARACONTROL_H

#include "backwindow.h"
#include "chooseimg.h"
using namespace std;

class para
{
public:
    para();
    ~para();
    void updataPara(Type type, int choosenum, int iteration, float hscaling, float vscaling, int min, int max, int factor);
    void updataType(Type type);
    void updataChoose(int choosenum);
    void updataItera(int iteration);
    void updataHscaling(int hsacling);
    void updataVscaling(int vscaling);
    void updataThreshold(int min, int max, int factor);
    void PassMaskAndSrc();
    void startPossion(int times, Type type);
    Type GetType();
    int GetChoose();
    int GetIteration();
    float GetHscaling();
    float GetVscaling();
    void ChangeChoose(int choosenum);

    backwindow *scr;
    backwindow *dst;
    ChooseImg *chooseimg = new ChooseImg;

    int max,min;
    int R, G, B;
    int ope;
    float alpha, beta;

private:
    Type type;
    int choosenum;
    int iteration;
    float hscaling;
    float vscaling;
    int factor;
    int startx;
    int starty;
    int imgWidge;
    int imgHeight;
};
#endif // PARACONTROL_H
