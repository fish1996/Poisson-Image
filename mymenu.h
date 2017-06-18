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
   void operatingJudge(int opeID);
   void ChooseJudge(int chooseID);
   void IneraJudge(int inera);
   void HscalingJudge(int hs);
   void VscalingJudge(int vs);
   void PushChooseButton();
   void PushReButton();
   void PushStartButton();

private:
   void createAction();        //创建动作
   void createMenu();          //创建菜单
   void createContentMenu();   //创建上下文菜单
   void createButtons();
   void drawButtons();

private:
   para *mainpara;

   QButtonGroup *operatingGroup;
   QRadioButton *SeamlessCloning;
   QRadioButton *GradientMixing;
   QRadioButton *TextureSmoothing;
   QRadioButton *LightingChange;
   QRadioButton *ColorChange;

   QButtonGroup *ChooseGroup;
   QRadioButton *RectangleChoose;
   QRadioButton *PersonChoose;

   QAction *ChooseSrcAction;    //选择原图
   QAction *ChooseDstAction;    //选择目标图像
   QAction *SavePhotoAction;    //保存目标图像

   QMenu *file;                //菜单

   QSpinBox *spinBox;

   QSlider *Hscaling;
   QSlider *Vscaling;

   QPushButton *ChooseMark;
   QPushButton *ReChoose;
   QPushButton *StartPossion;

   QLayout *OriImage;
   QLayout *TarImage;

   //QLabel *chooseimg = new QLabel;
};
#endif // MYMENU_H
