#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <dirent.h>
#include <QGridLayout>

using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    loadData();
}

MainWindow::~MainWindow() {
    histograms.clear();
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

    // load hisotrams to RAM
    histograms.resize(histograms.size() + filesList.size());
    cout << "histograms size is now " << histograms.size() << endl;
    cout << "\t> Importing images..." << endl;

    time.start();
    #pragma omp parallel for schedule(dynamic, omp_get_num_threads())
    for(int i=0; i<filesList.size(); i++) {
        QFileInfo fileInfo = filesList[i];
        QFile::copy(fileInfo.absoluteFilePath(), QString::fromStdString(DB_IMG_LOC + "img_" + format("%06d", i+identifier) + ".jpg"));
        items.append(QString::fromStdString("img_" + format("%06d", i+identifier) + ".jpg"));

        //cout << "\tGenerating histogram " << i+identifier << "\tThread: " << omp_get_thread_num() << endl;
        histograms[i+(identifier-1)] =  hm->extractHistogram(DB_IMG_LOC + "img_"  + format("%06d", i+identifier) + ".jpg",
                    DB_HIST_LOC  + "hist_" + format("%06d", i+identifier) + ".xml");
    }
    cout << "Elapsed time: " << time.elapsed() << endl;

    // update + store identifier on disc
    this->identifier += filesList.size();

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

    Mat img;
    img = imread(s.toStdString(), CV_LOAD_IMAGE_COLOR);
    cvtColor(img, img, CV_BGR2RGB);
    QImage image = QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
    image = image.scaled(ui->label->width(),ui->label->height());

    ui->label->setPixmap(QPixmap::fromImage(image));
    ui->label->show();

    /* selected image doesn't have to belong to the db so we must extract its histogram */
    hist_data selected = hm->extractHistogram(s.toStdString(), "selected.xml");
    ///cout << "SELETED IMAGE is " << selected.fname.c_str() << endl;

    // let's compare image histograms...
    vector< pair<int, double> > result(identifier);
    time.start();
    #pragma omp parallel for schedule(dynamic, omp_get_num_threads())
    for (int i=0; i < histograms.size(); i++){
        result[i] = make_pair(
            i, hm->compareHistograms(selected, histograms[i], 3)
        );
    }
    cout << "Elapsed time: " << time.elapsed() << endl;
    remove("selected.xml"); // delete file

    cout << "=============== RESULTS ===============" << endl;
    sort(result.begin(), result.end(), custom_sort);
    QList<QString> results;

    // get absolute path for each of N=4 top matches
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
    imagesWidget->setWindowTitle("TOP 4 COINCIDENCES");
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
 * @brief createTree
 */
void MainWindow::loadData(){
    if(!QDir("../db").exists()){
        this->identifier = 1;
        QDir().mkdir("../db");
        if(!QDir("../db/images").exists()) QDir().mkdir("../db/images");
        if(!QDir("../db/histograms").exists()) QDir().mkdir("../db/histograms");

    } else { // tree exists
        ifstream in;
        in.open ("../db/.id");
        if(in.is_open()){   //OK!

            // retrieve identifier!
            in >> this->identifier;

            QStringList img_list, hist_list;
            getDir(hist_list, XML);

            // load hisotrams to RAM
            this->histograms.resize(hist_list.size());
            time.start();
            #pragma omp parallel for schedule(dynamic, omp_get_num_threads())
            for(int i=0; i<hist_list.size(); i++){
                //cout << i << "\t > Loading HIST from " << hist_list.at(i).toStdString() << "\tThread: " << omp_get_thread_num() << endl;
                histograms[i]= hm->loadHistogram(hist_list.at(i).toStdString());
            }
            cout << "Elapsed time: " << time.elapsed() << endl;

            // load images
            getDir(img_list, JPG);
            ui->listWidget->addItems(img_list);
            ui->listWidget->setEnabled(true);
            ui->selectImage->setEnabled(true);
            in.close();

        } else this->identifier = 1; // in case of '.id' file error
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
        location = QString::fromStdString(this->DB_HIST_LOC);
        ext = ".xml";
    } else {
        location = QString::fromStdString(this->DB_IMG_LOC);
        ext = ".jpg";
    }

    if((dp  = opendir(location.toStdString().c_str())) == NULL) {
        cout << "Error opening " << endl;
    }

    // retrieve file into fileList
    while ((dirp = readdir(dp)) != NULL) {
        int len = strlen(dirp->d_name);
        if (len >= 4) {
            if (strcmp (ext.c_str(), &(dirp->d_name[len-4])) == 0) {
                if (type == XML) fileList << (location + QString::fromStdString(dirp->d_name));
                else fileList << QString::fromStdString(dirp->d_name);
            }
        }
    }

    closedir(dp);
    sort(fileList.begin(), fileList.end());
}
