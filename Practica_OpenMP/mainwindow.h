#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//#include <omp.h>
#include <QMainWindow>
#include<QFileDialog>
#include <fstream>
#include <string>
#include <strstream>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <histogramManager.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_loadDatabase_triggered();

    void on_selectImage_triggered();

private:
    int identifier;
    histogramManager *hm;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
