#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//#include <omp.h>
#include <QMainWindow>
#include <QApplication>
#include <QImage>
#include <QLabel>
#include <QList>
#include <QString>
#include <QFileDialog>
#include <QListWidgetItem>
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
    void on_loadDatabase_triggered();
    void on_selectImage_triggered();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    int identifier;
    QString dbLocation;
    histogramManager *hm;
    Ui::MainWindow *ui;

    void showResults();
};

#endif // MAINWINDOW_H
