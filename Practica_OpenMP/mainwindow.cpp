#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <dirent.h>
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

/**
 * DATABASE CREATION
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
        Mat img;
        img = imread(tempFileName.toStdString(), CV_LOAD_IMAGE_COLOR);

        string number = format("%06d", identifier++);
        string pathIm = this->dbImageLocation.toStdString() + "img_"+number+".jpg";

        items.append(QString::fromStdString("img_"+number+".jpg"));
        string pathHist = this->dbHistLocation.toStdString()+"hist_"+number+".xml";
        imwrite(pathIm, img);

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
    cvtColor(img, img, CV_BGR2RGB);
    QImage image = QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
    image = image.scaled(ui->label->width(),ui->label->height());

    //cout << "Selecting image " << path.toStdString().c_str() << endl;
    ui->label->setPixmap(QPixmap::fromImage(image));
    ui->label->show();

    /// selected image doesn't have to belong to the db so we have to extract its histogram
    hm->extractHistogram(s.toStdString(),this->dbHistLocation.toStdString()+"selected.xml");
    QList<QString> filesList;
    getDir(filesList, XML);

    /// let's compare image histograms...
    //map<int, double> results;
    vector< pair<double, int> > result;
    for (int i=0; i<filesList.size(); i++){
        result.push_back(make_pair(
                hm->compareHistograms(this->dbHistLocation.toStdString() + "selected.xml", filesList.at(i).toStdString(), 3), i));
    }

    sort(result.begin(), result.end());
    for (int i=result.size()-1; i>=0; i--){
        cout << result[i].second << "\t" << result[i].first << endl;
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
 * creates file-tree if needed.
 * persistence for database using file .id
 * @brief createTree
 */
void MainWindow::loadData(){
    if(!QDir("../db").exists()){
        this->identifier = 1;
        QDir().mkdir("../db");
        if(!QDir("../db/images").exists()) QDir().mkdir("../db/images");
        if(!QDir("../db/histograms").exists()) QDir().mkdir("../db/histograms");
    } else {

        /// retrieve identifier!
        ifstream in;
        in.open ("../db/.id");
        in >> this->identifier;
        in.close();

        /// load images
        QStringList backup;
        getDir(backup, JPG);
        ui->listWidget->addItems(backup);
        ui->listWidget->setEnabled(true);
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

    if (type == XML){
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
                cout << "added " <<  (location + QString::fromStdString(dirp->d_name)).toStdString().c_str() << endl;
            }
        }
    }
    closedir(dp);
    sort(fileList.begin(), fileList.end());
}
