#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    back = new backwindow(this);
    ui->imageLayout->addWidget(back);

    back->width=300;
    back->height=300;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openpushButton_clicked()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this,
        tr("选择图像"),
        "",
        tr("Images (*.png *.bmp *.jpg *.tif *.GIF.*jpeg)")); //选择路径
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        back->open(filename);
    }
}

void MainWindow::on_clearpushButton_clicked()
{
    back->clear();
}

void MainWindow::on_finishpushButton_clicked()
{
    back->finish();
    //back->start();
}
