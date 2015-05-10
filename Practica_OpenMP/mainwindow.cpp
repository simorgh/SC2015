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


void MainWindow::on_loadDatabase_triggered()
{
    QString s = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                             QString(), tr("log files (*.txt *.log);;All Files (*)"));

    QList<QString> filesList;
    QStringList items;
    ifstream file(s.toStdString().c_str());
    string str;

    while (getline(file, str))
    {
        string absPath = "../Practica_OpenMP/images/"+str ;
        filesList << QString::fromUtf8(absPath.c_str());
    }

    QString tempFileName;
    QList<QImage> images;
    this->dbLocation = "../db/images/";
    identifier = 1;
    cout << "Loading images..." << endl;
    foreach(QFileInfo fileInfo, filesList) {
        tempFileName = fileInfo.absoluteFilePath();
        //items.append(fileInfo.fileName());
        Mat img;
        img = imread(tempFileName.toStdString(), CV_LOAD_IMAGE_COLOR);

        string number = format("%06d", identifier++);
        string pathIm = this->dbLocation.toStdString() + "img_"+number+".jpg";

        items.append(QString::fromStdString("img_"+number+".jpg"));
        string pathHist = "../bd/histograms/hist_"+number+".xml";
        imwrite(pathIm, img);
        cout<<"\tGenerating histogram for "<<pathIm.c_str()<< endl;
        hm->extractHistogram(tempFileName.toStdString(),pathHist);


    }

   ui->listWidget->addItems(items);
  // search_btn->setEnabled(true);
}

void MainWindow::on_selectImage_triggered()
{
    // TODO: Compare histograms
    QString s = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                                  QString(), tr("Image files (*.jpg *.png);;All Files (*)"));
    //TODO fill graphicsView area
     cout <<"select image trigered"<< endl;
     Mat img;
     img = imread(s.toStdString(), CV_LOAD_IMAGE_COLOR);
     QImage image = QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);

     //cout << "Selecting image " << path.toStdString().c_str() << endl;
     ui->label->setPixmap(QPixmap::fromImage(image));
     ui->label->show();
}

void MainWindow::showResults(){

    //QGridLayout *grid = new QGridLayout(this);
    QImage copy;
    QString tempFileName;
    QList<QImage> images;
    QList<QString> filesList;
    filesList << "../image1.png" << "../image2.png" << "../image3.png";

    foreach(QFileInfo fileInfo, filesList){
        tempFileName = fileInfo.absoluteFilePath();
        cout << tempFileName.toStdString() << endl;
        Mat img;
        img = imread(tempFileName.toStdString(), CV_LOAD_IMAGE_COLOR);
        QImage image = QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
        copy = image.scaled(200,200,Qt::KeepAspectRatio);
        images.append(copy);
    }

    for (int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            QPixmap p(QPixmap::fromImage(images[i]));
            QLabel *label = new QLabel(this);
            label->setPixmap(p);
            //ui->centralWidget->layout()->addWidget(label);
            //grid->addWidget(label, i, j);
        }
    }

    //setLayout(grid);
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item) {
     QString path = this->dbLocation+item->text();
     Mat img;
     img = imread(path.toStdString(), CV_LOAD_IMAGE_UNCHANGED );
     QImage image = QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
     image = image.scaled(ui->label->width(),ui->label->height());

     //cout << "Selecting image " << path.toStdString().c_str() << endl;
     ui->label->setPixmap(QPixmap::fromImage(image));
     ui->label->show();
}
