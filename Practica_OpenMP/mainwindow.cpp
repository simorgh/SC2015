#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::on_loadDatabase_triggered()
{
    QString s = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                             QString(), tr("log files (*.txt *.log);;All Files (*)"));

    QList<QString> filesList;
    ifstream file(s.toStdString().c_str());
    string str;

    while (getline(file, str))
    {
        string absPath = "../Practica_OpenMP/images/"+str ;
        filesList << QString::fromUtf8(absPath.c_str());
    }



    QString tempFileName;
    QList<QImage> images;

    identifier = 1;
    foreach(QFileInfo fileInfo, filesList) {
        tempFileName = fileInfo.absoluteFilePath();
        cout << tempFileName.toStdString() << endl;
        Mat img;
        img = imread(tempFileName.toStdString(), CV_LOAD_IMAGE_COLOR);

        string number = format("%06d", identifier++);
        string pathIm = "../bd/images/img_"+number+".jpg";
        string pathHist = "../bd/histograms/hist_"+number+".xml";
        imwrite(pathIm, img);
        hm->extractHistogram(tempFileName.toStdString(),pathHist);
    }


  // search_btn->setEnabled(true);
}

void MainWindow::on_selectImage_triggered()
{
    // TODO: Compare histograms
    QString s = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                                  QString(), tr("Image files (*.jpg *.png);;All Files (*)"));

    /*for (int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            QPixmap p(QPixmap::fromImage(images[i*j +j]));
            QLabel *label = new QLabel(imagesWidget);
            label->setPixmap(p);
            grid->addWidget(label, i, j);

        }
    }*/

}

