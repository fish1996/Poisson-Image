#include "mainwindow.h"
#include "mymenu.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    MyMenu w;
    w.show();

    return a.exec();
}
