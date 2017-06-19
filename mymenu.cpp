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
    SeamlessCloning = new QRadioButton(QStringLiteral("无缝克隆"));
    GradientMixing = new QRadioButton(QStringLiteral("梯度混合"));
    TextureSmoothing = new QRadioButton(QStringLiteral("纹理平滑"));
    LightingChange = new QRadioButton(QStringLiteral("光照改变"));
    ColorChange = new QRadioButton(QStringLiteral("颜色改变"));

    operatingGroup = new QButtonGroup;
    operatingGroup->addButton(SeamlessCloning,0);
    operatingGroup->addButton(GradientMixing,1);
    operatingGroup->addButton(TextureSmoothing,2);
    operatingGroup->addButton(LightingChange,3);
    operatingGroup->addButton(ColorChange,4);
    connect(operatingGroup, SIGNAL(buttonClicked (int)), this, SLOT(operatingJudge(int)) );

    RectangleChoose = new QRadioButton(QStringLiteral("矩形"));
    PersonChoose = new QRadioButton(QStringLiteral("自由"));

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

    ChooseMark = new QPushButton(QStringLiteral("选择"));
    ReChoose = new QPushButton(QStringLiteral("重画"));
    StartPossion = new QPushButton(QStringLiteral("开始"));
    connect(ChooseMark, SIGNAL(clicked()), this, SLOT(PushChooseButton()));
    connect(ReChoose, SIGNAL(clicked()), this, SLOT(PushReButton()));
    connect(StartPossion, SIGNAL(clicked()), this, SLOT(PushStartButton()));
}

void MyMenu::drawButtons()
{
    QLabel *RadLabel = new QLabel;
    RadLabel->setText(QStringLiteral("选择操作"));

    QVBoxLayout *RadButton = new QVBoxLayout;
    RadButton->addWidget(RadLabel);
    RadButton->addWidget(SeamlessCloning);
    RadButton->addWidget(GradientMixing);
    RadButton->addWidget(TextureSmoothing);
    RadButton->addWidget(LightingChange);
    RadButton->addWidget(ColorChange);

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

    QVBoxLayout *ControlLayout = new QVBoxLayout;
    ControlLayout->addLayout(RadButton);
    ControlLayout->addStretch();
    ControlLayout->addLayout(ChooseLayout);
    ControlLayout->addStretch();
    ControlLayout->addLayout(Itera);
    ControlLayout->addStretch();
    ControlLayout->addLayout(Slider);
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

void MyMenu::operatingJudge(int opeID)
{
    mainpara->updataOper(opeID);
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

void MyMenu::PushChooseButton()
{
    mainpara->scr->GetMask();
    mainpara->scr->finish();
    mainpara->chooseimg->GetImg(mainpara->scr->PutFitSrc());
    mainpara->chooseimg->setVisible(true);
    mainpara->chooseimg->CanMoveImg();
}

void MyMenu::PushReButton()
{
    mainpara->scr->clear();
    mainpara->chooseimg->setVisible(false);
    mainpara->chooseimg->CanMoveImg();
    //chooseimg->move(10,10);
}

void MyMenu::PushStartButton()
{
    Type type;
    switch (mainpara->GetOper()) {
    case 0:
        type = NORMAL;
        break;
    case 1:
        type = GRADIENT;
        break;
    case 2:
        type = TEXTURE;
        break;
    case 3:
        type = LIGHT;
        break;
    case 4:
        type = COLOR;
        break;
    default:
        break;
    }
    mainpara->chooseimg->setVisible(true);
    mainpara->startPossion(mainpara->GetIteration(), type);
}
