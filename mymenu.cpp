#include "mymenu.h"
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QIcon>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

MyMenu::MyMenu(QWidget *parent) :
   QMainWindow(parent)
{
   mainpara = new para();

   createAction();
   createMenu();
   createContentMenu();
   createButtons();
   drawButtons();
   this->resize(1400, 500);
}
/****************************************
* Qt中创建菜单和工具栏需要如下步骤：
* 1. 建立行为Aciton
* 2. 创建菜单并使它与一个行为关联
* 3. 创建工具条并使它与一个行为关联
*****************************************/
void MyMenu::createAction()
{
   //创建打开文件动作
   ChooseSrcAction = new QAction(QIcon(tr("images/open.ico")), QStringLiteral("打开原图像"), this);
   /*//设置打开文件动作的快捷键
   ChooseSrcAction->setShortcut(tr("Ctrl + O"));
    //设置打开文件动作的提示信息
   ChooseSrcAction->setStatusTip(QStringLiteral("打开一个文件"));*/
   //关联打开文件动作的信号和槽
   connect(ChooseSrcAction, SIGNAL(triggered()), this, SLOT(ChooseSrcActionSlot()));

   ChooseDstAction = new QAction(QIcon(tr("images/open.ico")), QStringLiteral("打开目标图像"), this);
   connect(ChooseDstAction, SIGNAL(triggered()), this, SLOT(ChooseDstActionSlot()));

   SavePhotoAction = new QAction(QIcon(tr("images/open.ico")), QStringLiteral("保存图像"), this);
   connect(SavePhotoAction, SIGNAL(triggered()), this, SLOT(SavePhotoActionSlot()));

   SeamlessCloning = new QAction(QIcon(tr("images/open.ico")), QStringLiteral("无缝克隆"), this);
   connect(SeamlessCloning, SIGNAL(triggered()), this, SLOT(SeamlessCloningSlot()));

   OptimisedCloning = new QAction(QIcon(tr("images/open.ico")), QStringLiteral("优化无缝"), this);
   connect(OptimisedCloning, SIGNAL(triggered()), this, SLOT(OptimisedCloningSlot()));

   GradientMixing = new QAction(QIcon(tr("images/open.ico")), QStringLiteral("梯度混合"), this);
   connect(GradientMixing, SIGNAL(triggered()), this, SLOT(GradientMixingSlot()));

   TextureSmoothing = new QAction(QIcon(tr("images/open.ico")), QStringLiteral("纹理平滑"), this);
   connect(TextureSmoothing, SIGNAL(triggered()), this, SLOT(TextureSmoothingSlot()));

   LightingChange = new QAction(QIcon(tr("images/open.ico")), QStringLiteral("光照改变"), this);
   connect(LightingChange, SIGNAL(triggered()), this, SLOT(LightingChangeSlot()));

   ColorChange = new QAction(QIcon(tr("images/open.ico")), QStringLiteral("颜色改变"), this);
   connect(ColorChange, SIGNAL(triggered()), this, SLOT(ColorChangeSlot()));

   IntensityChange = new QAction(QIcon(tr("images/open.ico")), QStringLiteral("强度改变"), this);
   connect(IntensityChange, SIGNAL(triggered()), this, SLOT(IntensityChangeSlot()));

   SeamlessSplicing = new QAction(QIcon(tr("images/open.ico")), QStringLiteral("颜色迁移"), this);
   connect(SeamlessSplicing, SIGNAL(triggered()), this, SLOT(SeamlessSplicingSlot()));
}
void MyMenu::createMenu()
{
   /************************************************************************
    * menuBar第一次被调用时QMainWindow会产生一个QMenuBar在窗体上，且返回一个QMenu的指针
    * menuBar()->addMenu()会生成一个QMenu且返回他的指针
    * QMenu加入一个QAction, 就可以对事件进行反应了
    * 一个QAction可以被多个地方使用, 与Java的Action一样
    **************************************************************************/
   file = this->menuBar()->addMenu(QStringLiteral("文件"));
   file->addAction(ChooseSrcAction);
   file->addAction(ChooseDstAction);
   file->addAction(SavePhotoAction);

   ope = this->menuBar()->addMenu(QStringLiteral("操作"));
   ope->addAction(SeamlessCloning);
   ope->addAction(GradientMixing);
   ope->addAction(TextureSmoothing);
   ope->addAction(LightingChange);
   ope->addAction(ColorChange);
   ope->addAction(OptimisedCloning);
   ope->addAction(IntensityChange);
   ope->addAction(SeamlessSplicing);
}

void MyMenu::createContentMenu()
{
   this->addAction(ChooseSrcAction);
   this->addAction(ChooseDstAction);
   this->addAction(SavePhotoAction);
   //设置如何显示上下文菜单
   this->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MyMenu::createButtons()
{
    RectangleChoose = new QRadioButton(QStringLiteral("矩形"));
    PersonChoose = new QRadioButton(QStringLiteral("自由"));
    RectangleChoose->setChecked(true);

    ChooseGroup = new QButtonGroup;
    ChooseGroup->addButton(RectangleChoose,0);
    ChooseGroup->addButton(PersonChoose,1);
    connect(ChooseGroup, SIGNAL(buttonClicked(int)), this, SLOT(ChooseJudge(int)));

    spinBox = new QSpinBox;
    spinBox->setRange(1, 10000);
    spinBox->setValue(1000);
    connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(IneraJudge(int)));

    Hscaling = new QSlider(Qt::Horizontal);
    Hscaling->setRange(0,400);
    Hscaling->setValue(200);
    connect(Hscaling, SIGNAL(valueChanged(int)), this, SLOT(HscalingJudge(int)));

    Vscaling = new QSlider(Qt::Horizontal);
    Vscaling->setRange(0,400);
    Vscaling->setValue(200);
    connect(Vscaling, SIGNAL(valueChanged(int)), this, SLOT(VscalingJudge(int)));

    MinT = new QSlider(Qt::Horizontal);
    MinT->setRange(0,150);
    MinT->setValue(5);
    MinTBox = new QSpinBox;
    MinTBox->setRange(0,150);
    MinTBox->setValue(5);
    connect(MinT, SIGNAL(valueChanged(int)), this, SLOT(MinTJudge(int)));
    connect(MinT, SIGNAL(valueChanged(int)),MinTBox, SLOT(setValue(int)));
    connect(MinTBox, SIGNAL(valueChanged(int)),MinT, SLOT(setValue(int)));

    MaxT = new QSlider(Qt::Horizontal);
    MaxT->setRange(150,500);
    MaxT->setValue(200);
    MaxTBox = new QSpinBox;
    MaxTBox->setRange(150,500);
    MaxTBox->setValue(200);
    connect(MaxT, SIGNAL(valueChanged(int)), this, SLOT(MaxTJudge(int)));
    connect(MaxT, SIGNAL(valueChanged(int)),MaxTBox, SLOT(setValue(int)));
    connect(MaxTBox, SIGNAL(valueChanged(int)),MaxT, SLOT(setValue(int)));

    Operator3 = new QRadioButton("3");
    Operator5 = new QRadioButton("5");
    Operator7 = new QRadioButton("7");
    Operator3->setChecked(true);

    OperatorGroup = new QButtonGroup;
    OperatorGroup->addButton(Operator3,3);
    OperatorGroup->addButton(Operator5,5);
    OperatorGroup->addButton(Operator7,7);
    connect(OperatorGroup, SIGNAL(buttonClicked(int)), this, SLOT(OperatorJudge(int)));

    alphaL = new QSlider(Qt::Horizontal);
    alphaL->setRange(0,20);
    alphaL->setValue(2);
    ALBox = new QSpinBox;
    ALBox->setRange(0,20);
    ALBox->setValue(2);
    connect(alphaL, SIGNAL(valueChanged(int)), this, SLOT(alphaLJudge(int)));
    connect(alphaL, SIGNAL(valueChanged(int)),ALBox, SLOT(setValue(int)));
    connect(ALBox, SIGNAL(valueChanged(int)),alphaL, SLOT(setValue(int)));

    bateL = new QSlider(Qt::Horizontal);
    bateL->setRange(0,20);
    bateL->setValue(4);
    BLBox = new QSpinBox;
    BLBox->setRange(0,20);
    BLBox->setValue(4);
    connect(bateL, SIGNAL(valueChanged(int)), this, SLOT(bateLJudge(int)));
    connect(bateL, SIGNAL(valueChanged(int)),BLBox, SLOT(setValue(int)));
    connect(BLBox, SIGNAL(valueChanged(int)),bateL, SLOT(setValue(int)));

    RedC = new QSpinBox;
    RedC->setRange(0,255);
    RedC->setValue(0);
    connect(RedC, SIGNAL(valueChanged(int)), this, SLOT(RJudge(int)));

    GreenC = new QSpinBox;
    GreenC->setRange(0,255);
    GreenC->setValue(0);
    connect(GreenC, SIGNAL(valueChanged(int)), this, SLOT(GJudge(int)));

    BlueC = new QSpinBox;
    BlueC->setRange(0,255);
    BlueC->setValue(0);
    connect(RedC, SIGNAL(valueChanged(int)), this, SLOT(BJudge(int)));

    ChooseMark = new QPushButton(QStringLiteral("选择"));
    ReChoose = new QPushButton(QStringLiteral("重画"));
    StartPossion = new QPushButton(QStringLiteral("开始"));
    connect(ChooseMark, SIGNAL(clicked()), this, SLOT(PushChooseButton()));
    connect(ReChoose, SIGNAL(clicked()), this, SLOT(PushReButton()));
    connect(StartPossion, SIGNAL(clicked()), this, SLOT(PushStartButton()));
}

void MyMenu::drawButtons()
{
    Type tempT = mainpara->GetType();

    /*RadLabel = new QLabel;
    QLabel *RadLabel = new QLabel;
    if(tempT == NORMAL)
        RadLabel->setText(QStringLiteral("无缝克隆"));
    else if(tempT == MIXED)
        RadLabel->setText(QStringLiteral("梯度混合"));
    else if(tempT == TEXTURE)
        RadLabel->setText(QStringLiteral("纹理平滑"));
    else if(tempT == COLOR)
        RadLabel->setText(QStringLiteral("颜色改变"));
    else if(tempT == LIGHT)
        RadLabel->setText(QStringLiteral("光照改变"));
    else if(tempT == GRAY)
        RadLabel->setText(QStringLiteral("强度改变"));
    else if(tempT == OPTIMISED)
        RadLabel->setText(QStringLiteral("优化无缝"));
    else if(tempT == SPLICING)
        RadLabel->setText(QStringLiteral("无缝拼接"));*/

    QLabel *IteraLabel = new QLabel;
    IteraLabel->setText(QStringLiteral("迭代次数"));

    QLabel *ChooseLabel = new QLabel;
    ChooseLabel->setText(QStringLiteral("选图方式"));

    QHBoxLayout *ChooseButton = new QHBoxLayout;
    ChooseButton->addWidget(RectangleChoose);
    ChooseButton->addWidget(PersonChoose);

    QVBoxLayout *ChooseLayout = new QVBoxLayout;
    ChooseLayout->addWidget(ChooseLabel);
    ChooseLayout->addLayout(ChooseButton);

    QHBoxLayout *Itera = new QHBoxLayout;
    Itera->addWidget(IteraLabel);
    Itera->addWidget(spinBox);

    QLabel *HSLabel = new QLabel;
    HSLabel->setText(QStringLiteral("横向缩放"));

    QHBoxLayout *HSLayout = new QHBoxLayout;
    HSLayout->addWidget(HSLabel);
    HSLayout->addWidget(Hscaling);
    qDebug()<<"test";
    QLabel *VSLabel = new QLabel;
    VSLabel->setText(QStringLiteral("纵向缩放"));

    QHBoxLayout *VSLayout = new QHBoxLayout;
    VSLayout->addWidget(VSLabel);
    VSLayout->addWidget(Vscaling);

    QVBoxLayout *Slider = new QVBoxLayout;
    Slider->addLayout(HSLayout);
    Slider->addLayout(VSLayout);

    QVBoxLayout *ButtonLayout = new QVBoxLayout;
    ButtonLayout->addWidget(ChooseMark);
    ButtonLayout->addWidget(ReChoose);
    ButtonLayout->addWidget(StartPossion);

    QLabel *MinLabel = new QLabel;
    MinLabel->setText(QStringLiteral("最小阈值"));

    QHBoxLayout *MinLayout = new QHBoxLayout;
    MinLayout->addWidget(MinLabel);
    MinLayout->addWidget(MinT);
    MinLayout->addWidget(MinTBox);

    QLabel *MaxLabel = new QLabel;
    MaxLabel->setText(QStringLiteral("最大阈值"));

    QHBoxLayout *MaxLayout = new QHBoxLayout;
    MaxLayout->addWidget(MaxLabel);
    MaxLayout->addWidget(MaxT);
    MaxLayout->addWidget(MaxTBox);

    QVBoxLayout *ThresLayout = new QVBoxLayout;
    ThresLayout->addLayout(MinLayout);
    ThresLayout->addLayout(MaxLayout);

    QLabel *OperatorLabel = new QLabel;
    OperatorLabel->setText(QStringLiteral("算子大小"));

    QHBoxLayout *OperatorButton = new QHBoxLayout;
    OperatorButton->addWidget(Operator3);
    OperatorButton->addWidget(Operator5);
    OperatorButton->addWidget(Operator7);

    QVBoxLayout *OperatorLayout = new QVBoxLayout;
    OperatorLayout->addWidget(OperatorLabel);
    OperatorLayout->addLayout(OperatorButton);

    QLabel *AlphaLabel = new QLabel;
    AlphaLabel->setText(QStringLiteral("透明度"));

    QHBoxLayout *AlphaLayout = new QHBoxLayout;
    AlphaLayout->addWidget(AlphaLabel);
    AlphaLayout->addWidget(alphaL);
    AlphaLayout->addWidget(ALBox);

    QLabel *BateLabel = new QLabel;
    BateLabel->setText(QStringLiteral("软化"));

    QHBoxLayout *BateLayout = new QHBoxLayout;
    BateLayout->addWidget(BateLabel);
    BateLayout->addWidget(bateL);
    BateLayout->addWidget(BLBox);

    QVBoxLayout *LightLayout = new QVBoxLayout;
    LightLayout->addLayout(AlphaLayout);
    LightLayout->addLayout(BateLayout);

    QLabel *ColorLabel = new QLabel;
    ColorLabel->setText(QStringLiteral("调整颜色"));

    QLabel *RedLabel = new QLabel;
    RedLabel->setText("R");

    QHBoxLayout *RedLayout = new QHBoxLayout;
    RedLayout->addWidget(RedLabel);
    RedLayout->addWidget(RedC);

    QLabel *GreenLabel = new QLabel;
    GreenLabel->setText("G");

    QHBoxLayout *GreenLayout = new QHBoxLayout;
    GreenLayout->addWidget(GreenLabel);
    GreenLayout->addWidget(GreenC);

    QLabel *BlueLabel = new QLabel;
    BlueLabel->setText("B");

    QHBoxLayout *BlueLayout = new QHBoxLayout;
    BlueLayout->addWidget(BlueLabel);
    BlueLayout->addWidget(BlueC);

    QVBoxLayout *ColorLayout = new QVBoxLayout;
    ColorLayout->addWidget(ColorLabel);
    ColorLayout->addLayout(RedLayout);
    ColorLayout->addLayout(GreenLayout);
    ColorLayout->addLayout(BlueLayout);

    QVBoxLayout *ControlLayout = new QVBoxLayout;
    //ControlLayout->addWidget(RadLabel);
    //ControlLayout->addStretch();
    ControlLayout->addLayout(ChooseLayout);
    ControlLayout->addStretch();
    ControlLayout->addLayout(Itera);
    ControlLayout->addStretch();
    ControlLayout->addLayout(Slider);
    ControlLayout->addStretch();
    ControlLayout->addLayout(ThresLayout);
    ControlLayout->addStretch();
    ControlLayout->addLayout(OperatorLayout);
    ControlLayout->addStretch();
    ControlLayout->addLayout(ColorLayout);
    ControlLayout->addStretch();
    ControlLayout->addLayout(LightLayout);
    ControlLayout->addStretch();
    ControlLayout->addLayout(ButtonLayout);
    ControlLayout->addStretch();
    ControlLayout->addStretch();

    OriImage = new QHBoxLayout;
    TarImage = new QHBoxLayout;
    OriImage->addWidget(mainpara->scr);
    mainpara->scr->width=600;
    mainpara->scr->height=500;
    TarImage->addWidget(mainpara->dst);
    mainpara->dst->width = 600;
    mainpara->dst->height = 500;

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addStretch();
    mainLayout->addLayout(ControlLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(OriImage);
    mainLayout->addLayout(TarImage);
    mainLayout->setStretchFactor(ControlLayout, 1);
    mainLayout->setStretchFactor(OriImage, 3);
    mainLayout->setStretchFactor(TarImage, 3);

    QWidget* widget = new QWidget(this);
    widget->setLayout(mainLayout);
    this->setCentralWidget(widget);
    //updataButtons(NORMAL);
}

void MyMenu::ChooseSrcActionSlot()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this,
        QStringLiteral("选择图像"),
        "",
        tr("Images (*.png *.bmp *.jpg *.tif *.GIF *jpeg)")); //选择路径
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        mainpara->scr->open(filename);
    }
}

void MyMenu::ChooseDstActionSlot()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this,
        QStringLiteral("选择图像"),
        "",
        tr("Images (*.png *.bmp *.jpg *.tif *.GIF *jpeg)")); //选择路径
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        mainpara->dst->open(filename);
    }
}

void MyMenu::SavePhotoActionSlot()
{
     QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("你点击了~保存文件~菜单"), QMessageBox::Yes | QMessageBox::No);
}

void MyMenu::SeamlessCloningSlot()
{
    if(mainpara->GetType() != NORMAL){
        mainpara->updataType(NORMAL);
    }
}

void MyMenu::GradientMixingSlot()
{
    if(mainpara->GetType() != MIXED){
        mainpara->updataType(MIXED);
    }
}

void MyMenu::TextureSmoothingSlot()
{
    if(mainpara->GetType() != TEXTURE){
        mainpara->updataType(TEXTURE);
    }
}

void MyMenu::LightingChangeSlot()
{
    if(mainpara->GetType() != LIGHT){
        mainpara->updataType(LIGHT);
    }
}

void MyMenu::ColorChangeSlot()
{
    if(mainpara->GetType() != COLOR){
        mainpara->updataType(COLOR);
    }
}

void MyMenu::OptimisedCloningSlot()
{
    if(mainpara->GetType() != OPTIMISED){
        mainpara->updataType(OPTIMISED);
    }
}

void MyMenu::IntensityChangeSlot()
{
    if(mainpara->GetType() != GRAY){
        mainpara->updataType(GRAY);
    }
}

void MyMenu::SeamlessSplicingSlot()
{
    if(mainpara->GetType() != TRANSFER){
        mainpara->updataType(TRANSFER);
    }
}

void MyMenu::ChooseJudge(int chooseID)
{
    mainpara->updataChoose(chooseID);
}

void MyMenu::IneraJudge(int inera)
{
    mainpara->updataItera(inera);
}

void MyMenu::HscalingJudge(int hs)
{
    mainpara->updataHscaling(hs);
    mainpara->chooseimg->ChangeW(mainpara->GetHscaling());
}

void MyMenu::VscalingJudge(int vs)
{
    mainpara->updataVscaling(vs);
    mainpara->chooseimg->ChangeH(mainpara->GetVscaling());
}

void MyMenu::MinTJudge(int min)
{
    mainpara->min = min;
}

void MyMenu::MaxTJudge(int max)
{
    mainpara->max = max;
}

void MyMenu::MinTBoxJudge(int min)
{
    mainpara->min = min;
}

void MyMenu::MaxTBoxJudge(int max)
{
    mainpara->max = max;
}

void MyMenu::OperatorJudge(int oper)
{
    mainpara->oper = oper;
    qDebug("ope:%d",oper);
}

void MyMenu::alphaLJudge(int alpha)
{
    mainpara->alpha = alpha / 10.0;
}

void MyMenu::bateLJudge(int bate)
{
    mainpara->beta = bate / 10.0;
}

void MyMenu::ALBoxJudge(int alpha)
{
    mainpara->alpha = alpha / 10.0;
}

void MyMenu::BLBoxJudge(int bate)
{
    mainpara->beta = bate / 10.0;
}

void MyMenu::RJudge(int R)
{
    mainpara->R = R;
}

void MyMenu::GJudge(int G)
{
    mainpara->G = G;
}

void MyMenu::BJudge(int B)
{
    mainpara->B = B;
}

void MyMenu::PushChooseButton()
{
    Type tmp = mainpara->GetType();
    mainpara->scr->GetMask();
    mainpara->scr->finish();
    mainpara->chooseimg->GetImg(mainpara->scr->PutFitALP());
    if(tmp == NORMAL || tmp == MIXED || tmp == GRAY || tmp == OPTIMISED)
        mainpara->chooseimg->setVisible(true);
    mainpara->chooseimg->CanMoveImgP();
}

void MyMenu::PushReButton()
{
    mainpara->scr->clear();
    mainpara->chooseimg->setVisible(false);
    mainpara->chooseimg->CanMoveImgR();
    //chooseimg->move(10,10);
}

void MyMenu::PushStartButton()
{
    mainpara->startPossion(mainpara->GetIteration(), mainpara->GetType());
    mainpara->scr->clear();
}
