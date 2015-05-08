#pragma once

//#include <omp.h>
#include <QWidget>
#include <QPushButton>
#include <QApplication>
#include <QGridLayout>
#include <QFileInfo>
#include <QFileDialog>
#include <QLabel>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <histogramManager.h>

class QMainMenu : public QWidget {
Q_OBJECT

     public:
       QMainMenu(QWidget *parent = 0);
       histogramManager *hm;
       int identifier;

    private slots:
        void OnShowPressed();

    private:
        QPushButton *show_btn;
        QPushButton *empty_btn;
        QFileDialog *fileChooser;
};
