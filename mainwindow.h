#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFileDialog>

#include "backwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    backwindow *back;

private slots:
    void on_openpushButton_clicked();

    void on_clearpushButton_clicked();

private:
    Ui::MainWindow *ui;


};

#endif // MAINWINDOW_H

