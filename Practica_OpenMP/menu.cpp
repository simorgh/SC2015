#include "menu.h"
#include <fstream>
#include <string>
#include <strstream>

using namespace std;
using namespace cv;

/**
 * @brief QMainMenu::QMainMenu
 * @param parent
 */
QMainMenu::QMainMenu(QWidget *parent) : QWidget(parent) {
/*
 *  #pragma omp parallel {
        printf("Hello from thread %d, nthreads %d\n", omp_get_thread_num(), omp_get_num_threads());
    }
*/
    loaddb_btn = new QPushButton("Load database", this);
    loaddb_btn->move(20, 50);

    search_btn = new QPushButton("Search Image", this);
    search_btn->move(20, 80);


    //cout << s;
    connect(loaddb_btn, SIGNAL(clicked()), this, SLOT(OnShowPressed()));
    connect(search_btn, SIGNAL(clicked()), this, SLOT(OnSearchPressed()));
    //TODO; check if db is available and enable the button if it is
    search_btn->setEnabled(false);
}



/**
 * @brief QMainMenu::OnShowPressed
 */
void QMainMenu::OnShowPressed() {
    //fileChooser = new QFileDialog();
    QString s = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                                  QString(), tr("log files (*.txt *.log);;All Files (*)"));
    QWidget *imagesWidget = new QWidget();
    QGridLayout *grid = new QGridLayout(imagesWidget);

    QList<QString> filesList;
    ifstream file(s.toStdString().c_str());
    string str;

    while (getline(file, str))
    {
        string absPath = "../Practica_OpenMP/images/"+str ;
        filesList << QString::fromUtf8(absPath.c_str());
    }


    QImage copy;
    QString tempFileName;
    QList<QImage> images;

    identifier = 1;
    foreach(QFileInfo fileInfo, filesList) {
        tempFileName = fileInfo.absoluteFilePath();
        cout << tempFileName.toStdString() << endl;
        Mat img;
        img = imread(tempFileName.toStdString(), CV_LOAD_IMAGE_COLOR);

        stringstream ss;

        string number = format("%06d", identifier++);
        string pathIm = "../bd/images/img_"+number+".jpg";
        string pathHist = "../bd/histograms/hist_"+number+".xml";
        imwrite(pathIm, img);
        hm->extractHistogram(tempFileName.toStdString(),pathHist);
        printf("inserting  into database image %s\n",pathIm.c_str());
        //QImage image = QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
        //QImage image(tempFileName);
        //copy = image.scaled(200,200,Qt::KeepAspectRatio);
        //images.append(copy);
    }

    /*for (int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            QPixmap p(QPixmap::fromImage(images[i*j +j]));
            QLabel *label = new QLabel(imagesWidget);
            label->setPixmap(p);
            grid->addWidget(label, i, j);

        }
    }*/

    setLayout(grid);
    search_btn->setEnabled(true);
}

void QMainMenu::OnSearchPressed() {

    QString s = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                                  QString(), tr("Image files (*.jpg *.png);;All Files (*)"));
    QWidget *imagesWidget = new QWidget();
    QGridLayout *grid = new QGridLayout(imagesWidget);

    QList<QString> filesList;
    ifstream file(s.toStdString().c_str());
    string str;

    while (getline(file, str))
        {
        string absPath = "../Practica_OpenMP/images/"+str ;
        filesList << QString::fromUtf8(absPath.c_str());
        }


    QImage copy;
    QString tempFileName;
    QList<QImage> images;
    //QList<QString> filesList;
    //filesList << "../Practica_OpenMP/image1.png" << "../Practica_OpenMP/image2.png" << "../Practica_OpenMP/image3.png";
    identifier = 1;
    foreach(QFileInfo fileInfo, filesList) {
        tempFileName = fileInfo.absoluteFilePath();
        cout << tempFileName.toStdString() << endl;
        Mat img;
        img = imread(tempFileName.toStdString(), CV_LOAD_IMAGE_COLOR);

        stringstream ss;

        string number = format("%06d", identifier++);
        string pathIm = "../bd/images/img_"+number+".jpg";
        string pathHist = "../bd/histograms/hist_"+number+".xml";
        imwrite(pathIm, img);
        hm->extractHistogram(tempFileName.toStdString(),pathHist);
        printf("inserting  into database image %s\n",pathIm.c_str());
        //QImage image = QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
        //QImage image(tempFileName);
        //copy = image.scaled(200,200,Qt::KeepAspectRatio);
        //images.append(copy);
    }

    /*for (int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            QPixmap p(QPixmap::fromImage(images[i*j +j]));
            QLabel *label = new QLabel(imagesWidget);
            label->setPixmap(p);
            grid->addWidget(label, i, j);

        }
    }*/

    setLayout(grid);

}

