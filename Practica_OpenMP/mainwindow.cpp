#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <dirent.h>
#include <QGridLayout>

using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->dbHistLocation = "../db/histograms/";
    this->dbImageLocation = "../db/images/";
    loadData();
}

MainWindow::~MainWindow() {
    delete ui;
}

bool custom_sort(const pair<int, double> &a, const pair<int, double> &b){
    return a.second > b.second;
}

/**
 * DATABASE CREATION
 * -----------------------------------------------
 * logfile: find images -name *.jpg >> fileList.txt
 * readlink -f image.jpg
 *
 * @brief MainWindow::on_importDatabase_triggered
 */
void MainWindow::on_importDatabase_triggered() {
    QList<QString> filesList;
    QStringList items;
    QString s = QFileDialog::getOpenFileName(this, tr("Open File..."),
                    QString(), tr("log files (*.txt *.log);;All Files (*)"));

    ifstream file(s.toStdString().c_str());
    string str;
    while (getline(file, str)) filesList << QString::fromStdString("../"+str);
    file.close();

    QString tempFileName;
    cout << "Loading images..." << endl;
    foreach(QFileInfo fileInfo, filesList) {
        tempFileName = fileInfo.absoluteFilePath();
        string number = format("%06d", identifier++);

        /// copying images to db...
        Mat img;
        img = imread(tempFileName.toStdString(), CV_LOAD_IMAGE_COLOR);
        string nameIm = "img_" + number + ".jpg";
        string pathIm = this->dbImageLocation.toStdString() + nameIm;
        imwrite(pathIm, img);
        items.append(QString::fromStdString(nameIm));

        /// extracting img histogram ...
        string pathHist = this->dbHistLocation.toStdString() + "hist_" + number + ".xml";
        cout<<"\tGenerating histogram for " << pathIm.c_str() << endl;
        hm->extractHistogram(pathIm, pathHist);
    }

    /// store identifier on disc
    ofstream out;
    out.open ("../db/.id");
    out << this->identifier;
    out.close();

    ui->listWidget->addItems(items);
    ui->listWidget->setEnabled(true);
    ui->selectImage->setEnabled(true);
}

/**
 * COMPUTE MATCHING FUNC.
 * -----------------------------------------------
 * Generates selected image histogram, then it's compared
 * to db in order to get N best matches.
 *
 * @brief MainWindow::on_selectImage_triggered
 */
void MainWindow::on_selectImage_triggered() {
    QString s = QFileDialog::getOpenFileName(this, tr("Open File..."),
                          QString(), tr("Image files (*.jpg *.png);;All Files (*)"));
    if(s == NULL) return;

    cout <<"SELETED IMAGE is "<< s.toStdString().c_str()<< endl;
    Mat img;
    img = imread(s.toStdString(), CV_LOAD_IMAGE_COLOR);
    cvtColor(img, img, CV_BGR2RGB);
    QImage image = QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
    image = image.scaled(ui->label->width(),ui->label->height());

    //cout << "Selecting image " << path.toStdString().c_str() << endl;
    ui->label->setPixmap(QPixmap::fromImage(image));
    ui->label->show();

    /// selected image doesn't have to belong to the db so we must extract its histogram
    hm->extractHistogram(s.toStdString(),this->dbHistLocation.toStdString()+"selected.xml");
    QList<QString> filesList;
    getDir(filesList, XML);

    /// let's compare image histograms...
    vector< pair<int, double> > result(filesList.size());
#pragma omp parallel for
    for (int i=0; i<filesList.size(); i++){
        result[i] = (make_pair(
            i, hm->compareHistograms(this->dbHistLocation.toStdString() + "selected.xml", filesList.at(i).toStdString(), 3))
        );
    }


    //cout << "=============== RESULTS ===============" << endl;
    sort(result.begin(), result.end(), custom_sort);
    QList<QString> results;

    /// get absolute path for each of N=4 top matches
    int count =0, id = 0;
    while(count < 4){
        int num = (result[id].first) + 1;
        cout << "Num =  "<< num << endl;
        if(num < this->identifier) {
            results << this->dbImageLocation + QString::fromStdString("img_" + format("%06d", num) + ".jpg");
            count++;
        }

        id++;
    }

    this->showResults(results);
}

/**
 * DISPLAY TOP N MATCHES
 * -----------------------------------------------
 * @brief MainWindow::showResults
 * @param fileList
 */
void MainWindow::showResults(QList<QString> &fileList){
    if(fileList.empty()) return;
    QWidget *imagesWidget = new QWidget();
    QGridLayout *grid = new QGridLayout(imagesWidget);
    QImage copy;
    QString tempFileName;
    QList<QImage> images;

    foreach(QFileInfo fileInfo, fileList){
        tempFileName = fileInfo.absoluteFilePath();
        cout << tempFileName.toStdString() << endl;

        Mat img;
        img = imread(tempFileName.toStdString(), CV_LOAD_IMAGE_COLOR);
        cvtColor(img, img, CV_BGR2RGB);
        QImage image = QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
        copy = image.scaled(300, 200);

        images.append(copy);
    }

    int cont = 0;
    for (int i = 0; i < 2; i++) {
        for(int j = 0; j < 2; j++) {
            QPixmap p(QPixmap::fromImage(images[cont++]));
            QLabel *label = new QLabel(this);
            label->setPixmap(p);
            grid->addWidget(label, i, j);
        }
    }

    imagesWidget->setLayout(grid);
    imagesWidget->show();
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item) {
     QString path = this->dbImageLocation + item->text();
     Mat img;
     img = imread(path.toStdString(), CV_LOAD_IMAGE_UNCHANGED );
     cvtColor(img, img, CV_BGR2RGB);

     QImage image = QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
     image = image.scaled(ui->label->width(),ui->label->height());

     //cout << "Selecting image " << path.toStdString().c_str() << endl;
     ui->label->setPixmap(QPixmap::fromImage(image));
     ui->label->show();
}

/**
 * LOAD DATABASE TREE
 * -----------------------------------------------
 * creates file-tree if needed.
 * persistence for database using file .id
 * @brief createTreehome/vroigrip8.alumnes/sc2015/db/images/img_000055.jpg
/home/vroigrip8.alumnes/sc2015/db/images/img_000003.jpg
/home/vroigrip8.alumnes/sc2015/db/images/img_000052.jpg
/home/vroigrip8.alumnes/sc2015/db/images/img_000009.jpg
 */
void MainWindow::loadData(){
    if(!QDir("../db").exists()){
        this->identifier = 1;
        QDir().mkdir("../db");
        if(!QDir("../db/images").exists()) QDir().mkdir("../db/images");
        if(!QDir("../db/histograms").exists()) QDir().mkdir("../db/histograms");

    } else { /// tree exists
        ifstream in;
        in.open ("../db/.id");
        if(in.is_open()){   ///OK!
            /// retrieve identifier!
            in >> this->identifier;

            /// load images
            QStringList backup;
            getDir(backup, JPG);
            ui->listWidget->addItems(backup);
            ui->listWidget->setEnabled(true);
            ui->selectImage->setEnabled(true);
            in.close();
        } else this->identifier = 1; /// in case of '.id' file error
    }

}

/**
 * Fetch directory data depending on a given extension (.xml, .jpg)
 *
 * @brief MainWindow::getDir
 * @param fileList
 * @param type
 */
void MainWindow::getDir(QList<QString> &fileList, F_TYPE type) {
    DIR *dp;
    struct dirent *dirp;
    QString location;
    string ext;

    if(type == XML) {
        location = this->dbHistLocation;
        ext = ".xml";
    } else {
        location = this->dbImageLocation;
        ext = ".jpg";
    }

    if((dp  = opendir(location.toStdString().c_str())) == NULL) {
        cout << "Error opening "  << endl;
    }

    /// retrieve file into fileList
    while ((dirp = readdir(dp)) != NULL) {
        int len = strlen(dirp->d_name);
        if (len >= 4) {
            if (strcmp (ext.c_str(), &(dirp->d_name[len-4])) == 0) {
                if (type == XML) fileList << (location + QString::fromStdString(dirp->d_name));
                else fileList << QString::fromStdString(dirp->d_name);
                //cout << "added " <<  (location + QString::fromStdString(dirp->d_name)).toStdString().c_str() << endl;
            }
        }
    }
    closedir(dp);
    sort(fileList.begin(), fileList.end());
}
