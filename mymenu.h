#ifndef MYMENU_H
#define MYMENU_H
#include "backwindow.h"
#include "paracontrol.h"
#include <QMainWindow>
#include <QRadioButton>
#include <QButtonGroup>
#include <QSpinBox>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QVBoxLayout>

class QAction;
class QMenu;
class MyMenu : public QMainWindow
{
   Q_OBJECT
public:
   explicit MyMenu(QWidget *parent = 0);

signals:

public slots:
private slots:
   void ChooseSrcActionSlot();  //打开文件动作对应的槽函数
   void ChooseDstActionSlot(); //关闭文件动作对应的槽函数
   void SavePhotoActionSlot();
   void SeamlessCloningSlot();
   void GradientMixingSlot();
   void TextureSmoothingSlot();
   void LightingChangeSlot();
   void ColorChangeSlot();
   void OptimisedCloningSlot();
   void IntensityChangeSlot();
   void SeamlessSplicingSlot();
   void ChooseJudge(int chooseID);
   void IneraJudge(int inera);
   void HscalingJudge(int hs);
   void VscalingJudge(int vs);
   void MinTJudge(int min);
   void MaxTJudge(int max);
   void MinTBoxJudge(int min);
   void MaxTBoxJudge(int max);
   void OperatorJudge(int ope);
   void alphaLJudge(int alpha);
   void bateLJudge(int bate);
   void ALBoxJudge(int alpha);
   void BLBoxJudge(int bate);
   void RJudge(int R);
   void GJudge(int G);
   void BJudge(int B);
   void PushChooseButton();
   void PushReButton();
   void PushStartButton();

private:
   void createAction();        //创建动作
   void createMenu();          //创建菜单
   void createContentMenu();   //创建上下文菜单
   void createButtons();
   void drawButtons();
   void updataButtons(Type tempT, QVBoxLayout *Add);

private:
   para *mainpara;

   QAction *SeamlessCloning;
   QAction *GradientMixing;
   QAction *TextureSmoothing;
   QAction *LightingChange;
   QAction *ColorChange;
   QAction *OptimisedCloning;
   QAction *IntensityChange;
   QAction *SeamlessSplicing;

   QButtonGroup *ChooseGroup;
   QRadioButton *RectangleChoose;
   QRadioButton *PersonChoose;

   QButtonGroup *OperatorGroup;
   QRadioButton *Operator3;
   QRadioButton *Operator5;
   QRadioButton *Operator7;

   QAction *ChooseSrcAction;    //选择原图
   QAction *ChooseDstAction;    //选择目标图像
   QAction *SavePhotoAction;    //保存目标图像

   QMenu *file;                //菜单
   QMenu *ope;

   QSpinBox *spinBox;

   QSlider *Hscaling;
   QSlider *Vscaling;

   QSlider *MinT;
   QSlider *MaxT;
   QSpinBox *MinTBox;
   QSpinBox *MaxTBox;

   QSlider *alphaL;
   QSlider *bateL;
   QSpinBox *ALBox;
   QSpinBox *BLBox;

   QSpinBox *RedC;
   QSpinBox *GreenC;
   QSpinBox *BlueC;

   QPushButton *ChooseMark;
   QPushButton *ReChoose;
   QPushButton *StartPossion;

   QLayout *OriImage;
   QLayout *TarImage;

   QVBoxLayout *ControlLayout;
   //QLabel *chooseimg = new QLabel;
};
#endif // MYMENU_H
