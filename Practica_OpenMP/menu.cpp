#include "menu.h"

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
    show_btn = new QPushButton("Show images", this);
    show_btn->move(20, 50);

    empty_btn = new QPushButton("I do nothing :)", this);
    empty_btn->move(23, 80);

    connect(show_btn, SIGNAL(clicked()), this, SLOT(OnShowPressed()));
}


/**
 * @brief QMainMenu::OnShowPressed
 */
void QMainMenu::OnShowPressed() {
    QWidget *imagesWidget = new QWidget();
    QGridLayout *grid = new QGridLayout(imagesWidget);

    QImage copy;
    QString tempFileName;
    QList<QImage> images;
    QList<QString> filesList;
    filesList << "../Practica_OpenMP/image1.png" << "../Practica_OpenMP/image2.png" << "../Practica_OpenMP/image3.png";

    foreach(QFileInfo fileInfo, filesList) {
        tempFileName = fileInfo.absoluteFilePath();
        cout << tempFileName.toStdString() << endl;
        Mat img;
        img = imread(tempFileName.toStdString(), CV_LOAD_IMAGE_COLOR);
        QImage image = QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
        //QImage image(tempFileName);
        copy = image.scaled(200,200,Qt::KeepAspectRatio);
        images.append(copy);
    }

    for (int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            QPixmap p(QPixmap::fromImage(images[i]));
            QLabel *label = new QLabel(imagesWidget);
            label->setPixmap(p);
            grid->addWidget(label, i, j);
        }
    }

    setLayout(grid);
}
