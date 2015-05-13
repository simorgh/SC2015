#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <dirent.h>
using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    identifier = 1;
    this->dbHistLocation = "../db/histograms/";
    this->dbImageLocation = "../db/images/";
}

MainWindow::~MainWindow() {
    delete ui;
}

/**
 * DATABASE CREATION
 * logfile: find images -name *.jpg >> fileList.txt
 * readlink -f image.jpg
 *
 * @brief MainWindow::on_loadDatabase_triggered
 */
void MainWindow::on_loadDatabase_triggered() {
    QString s = QFileDialog::getOpenFileName(this, tr("Open File..."),
                    QString(), tr("log files (*.txt *.log);;All Files (*)"));

    QList<QString> filesList;
    QStringList items;
    ifstream file(s.toStdString().c_str());

    string str;
    while (getline(file, str)) {
        //string absPath = "../Practica_OpenMP/images/" + str ;
        filesList << QString::fromStdString("../"+str);
    }

    /* creates file-tree if needed */
    if(!QDir("../db").exists()){
        QDir().mkdir("../db");
        if(!QDir("../db/images").exists()) QDir().mkdir("../db/images");
        if(!QDir("../db/histograms").exists()) QDir().mkdir("../db/histograms");
    }



    QString tempFileName;
    cout << "Loading images..." << endl;
    foreach(QFileInfo fileInfo, filesList) {
        tempFileName = fileInfo.absoluteFilePath();
        Mat img;
        img = imread(tempFileName.toStdString(), CV_LOAD_IMAGE_COLOR);

        string number = format("%06d", identifier++);
        string pathIm = this->dbImageLocation.toStdString() + "img_"+number+".jpg";

        items.append(QString::fromStdString("img_"+number+".jpg"));
        string pathHist = this->dbHistLocation.toStdString()+"hist_"+number+".xml";
        imwrite(pathIm, img);

        cout<<"\tGenerating histogram for "<<pathIm.c_str()<< endl;
        hm->extractHistogram(tempFileName.toStdString(),pathHist);
    }

    ui->listWidget->addItems(items);
    ui->listWidget->setEnabled(true);
}

/**
 * Generates selected image histogram, then it's compared
 * to db in order to get N best matches.
 *
 * @brief MainWindow::on_selectImage_triggered
 */
void MainWindow::on_selectImage_triggered() {
    QString s = QFileDialog::getOpenFileName(this, tr("Open File..."),
                          QString(), tr("Image files (*.jpg *.png);;All Files (*)"));

    cout <<"select image trigered"<< s.toStdString().c_str()<< endl;
    Mat img;
    img = imread(s.toStdString(), CV_LOAD_IMAGE_COLOR);
    QImage image = QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);

    //cout << "Selecting image " << path.toStdString().c_str() << endl;
    ui->label->setPixmap(QPixmap::fromImage(image));
    ui->label->show();
    /*The selected image doesn't have to belong to the db so we have to extract its histogram first */
    hm->extractHistogram(s.toStdString(),this->dbHistLocation.toStdString()+"selected.xml");

    QList<QString> filesList;
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(this->dbHistLocation.toStdString().c_str())) == NULL) {
        cout << "Error opening "  << endl;
    }

    while ((dirp = readdir(dp)) != NULL) {
        int len = strlen (dirp->d_name);
        if (len >= 4) {
            if (strcmp (".xml", &(dirp->d_name[len - 4])) == 0) {
                //cout << "file read " << dirp->d_name << endl;
                filesList << (this->dbHistLocation+QString::fromStdString(dirp->d_name));
                cout << "added " <<  (this->dbHistLocation+QString::fromStdString(dirp->d_name)).toStdString().c_str() << endl;

            }
        }
    }
    closedir(dp);


    for (int i = 0; i < filesList.size(); i++){
        hm->compareHistograms(this->dbHistLocation.toStdString()+"selected.xml",filesList.at(i).toStdString(),3);
    }
}

/**
 * @brief MainWindow::showResults
 */
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
     QString path = this->dbImageLocation+item->text();
     Mat img;
     img = imread(path.toStdString(), CV_LOAD_IMAGE_UNCHANGED );
     QImage image = QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
     image = image.scaled(ui->label->width(),ui->label->height());

     //cout << "Selecting image " << path.toStdString().c_str() << endl;
     ui->label->setPixmap(QPixmap::fromImage(image));
     ui->label->show();
}

/**
 * @brief getdir
 * @param dir
 * @param files
 */
void getdir(string dir, vector<string> &files) {
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error opening " << dir << endl;

    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);

}

/**
 * @brief print_dir
 * @return
 */
int print_dir() {
    string dir = string(".");
    vector<string> files = vector<string>();

    getdir(dir,files);

    for (unsigned int i = 0;i < files.size();i++) {
        cout << files[i] << endl;
    }
    return 0;
}
