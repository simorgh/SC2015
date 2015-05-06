#pragma once

//#include <omp.h>
#include <QWidget>
#include <QPushButton>
#include <QApplication>
#include <QGridLayout>
#include <QFileInfo>
#include <QLabel>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

class QMainMenu : public QWidget {
Q_OBJECT

     public:
       QMainMenu(QWidget *parent = 0);

    private slots:
        void OnShowPressed();

    private:
        QPushButton *show_btn;
        QPushButton *empty_btn;
};
