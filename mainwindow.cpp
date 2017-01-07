/*
 *  File    : MainWindow.cpp
 *  Desc    :
 *  Author  : Ismail
 */

#include "mainwindow.h"
#include "Image.h"
#include "ui_mainwindow.h"

#include <QtGui>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    worker = new CVWorker();
    _labels = QString("ا;ب;ت;ث;ج;ح;خ;د;س;ف;ص;ء;ع;غ;ه;ل;بـ;تـ;ثـ;جـ;حـ;خـ;ذ;سـ;فـ;صـ;ئ;عـ;غـ;هـ;لـ;م;ن;ة;و;ي;ك;ر;ش;ق;ض;ئـ;ـعـ;ـغـ;ـهـ;لا;مـ;نـ;ـة;ؤ;يـ;كـ;ز;شـ;قـ;ضـ;ط;ظ;؟").split(";");
    // _labels = QString("ا;ب;ت;ث;ج;ح;خ;د;س;ف;ص;ط;ع;غ;ه;ل;بـ;تـ;ثـ;جـ;حـ;خـ;ذ;سـ;فـ;صـ;ى;عـ;غـ;هـ;لـ;م;ن;ة;و;ي;ظ;ر;ش;ق;ض;ئـ;ـعـ;ـغـ;ـهـ;لا;مـ;نـ;ـة;ؤ;يـ;كـ;ز;شـ;قـ;ضـ").split(";");

    setupUI();
    setupConnections();
}

void MainWindow::setupUI()
{
    ui->setupUi(this);
    ui->txtOutput->setReadOnly(true);
    ui->tblFreq->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tblFreq->verticalHeader()->setDefaultSectionSize(20);

   // ui->txtOutput->setPlainText("Output here\n===========");

    setWindowTitle("OCRabic");
}

void MainWindow::setupConnections()
{
    connect(ui->btnLoad, SIGNAL(clicked()), this, SLOT(receiveLoad()));
    connect(ui->btnSet, SIGNAL(clicked()), this, SLOT(testNNLearner()));

    connect(this, SIGNAL(sendLoadImages(QString)), worker, SLOT(receiveLoadImages(QString)));
    connect(worker, SIGNAL(sendLoadedImages(QVector<Image*>*)), this, SLOT(receiveLoadedImages(QVector<Image*>*)));

    // Logging and errors
    connect(this, SIGNAL(sendError(QString)), this, SLOT(receiveError(QString)));
    connect(this, SIGNAL(sendLog(QString)), this, SLOT(receiveLog(QString)));
    connect(worker, SIGNAL(sendError(QString)), this, SLOT(receiveError(QString)));
    connect(worker, SIGNAL(sendLog(QString)), this, SLOT(receiveLog(QString)));
}

void MainWindow::receiveLoad()
{
    /* QString dirname  = QFileDialog::getExistingDirectory(this, tr("Choose the data forlder"),
                                                         QDir::currentPath(), QFileDialog::ShowDirsOnly);*/

    QString dirname = "C:\\Users\\Ismail\\Desktop\\balanced\\img";
    // QString dirname = "C:\\Users\\Ismail\\Desktop\\minitest\\img";
    if(!dirname.isEmpty())
    {
        emit sendLoadImages(dirname);

        /* QString filename = QFileDialog::getOpenFileName(this, tr("Select the input file"),
                                                        "C:\\Users\\Ismail\\Desktop\\balanced\\out.txt");*/

        QString filename = "C:\\Users\\Ismail\\Desktop\\balanced\\out.txt";
        // QString filename = "C:\\Users\\Ismail\\Desktop\\minitest\\out.txt";
        if(!filename.isEmpty())
        {
            QFile f(filename);
            if(f.open(QIODevice::ReadOnly))
            {
                QTextStream in(&f);
                QString s;
                int nClasses = _labels.size();

                QMap<int, int> freq;
                for(int i = 0; i < _labels.size(); i++)
                    freq.insert(i, 0);

                emit sendLog("Loading labels ..");

                in >> s;
                while(!s.isEmpty())
                {
                    freq[s.toInt()]++;
                    in >> s;
                }

                emit sendLog("Labels loaded successfully!");
                f.close();

                int sum = 0;
                ui->tblFreq->setColumnCount(2);
                ui->tblFreq->setRowCount(nClasses + 1);
                for(int i = 0; i < nClasses; i++)
                {
                    ui->tblFreq->setItem(i, 0, new QTableWidgetItem(_labels[i]));
                    ui->tblFreq->setItem(i, 1, new QTableWidgetItem(QString("%1").arg(freq[i])));
                    sum += freq[i];
                }
                ui->tblFreq->setItem(nClasses, 0, new QTableWidgetItem("Total"));
                ui->tblFreq->setItem(nClasses, 1, new QTableWidgetItem(QString("%1").arg(sum)));
            }
            else
            {
                emit sendError("File can't be opened.");
            }
        }
    }
}


void MainWindow::testKNNLearner()
{
    int nFeatures = (*_images)[0]->cols * (*_images)[0]->rows;
    _knn = new KNNLearner(3);
    _knn->load(_images, nFeatures);
    _knn->test();
}

void MainWindow::testNNLearner()
{
    int nFeatures = (*_images)[0]->cols * (*_images)[0]->rows;
    _nn = new NNLearner(nFeatures, _labels.size(), nFeatures, 0.5, _labels.size(), 2);
    _nn->load(_images, 0.2);
    /*_nn = new NNLearner(nFeatures, _labels.size(), 3*nFeatures, 0.05, 54, 500);
    _nn->load(_images, 0.0);*/
    _nn->test();
}

void MainWindow::receiveLoadedImages(QVector<Image*>* images)
{
    if(images->size() > 0)
    {
        int nFeatures = (*images)[0]->cols * (*images)[0]->rows;
        ui->lblExamples->setText(QString("# Examples : %1").arg(images->size()));
        ui->lblFeatures->setText(QString("# Features : %1").arg(nFeatures));
        ui->lblClasses->setText(QString("# Classes : %1").arg(_labels.size()));
        _images = images;

        // testKNNLearner();
    }

    /*
    int counter = 0;
    for(Image* img : *images)
    {
        if(img->getClass() == 1 && counter++ < 20)
            ui->verticalLayout->addWidget(img->getQLabel());
    }
    */

    /*
    int nClasses = _labels.size();
    QMap<int, int> freq;
    for(int i = 0; i < nClasses; i++)
        freq.insert(i, 0);

    for(Image* img : *images)
        freq[img->getClass()]++;


    for(int i = 0; i < nClasses; i++)
    {
        sendLog(QString("%1 : %2").arg(i+1).arg(freq[i]));
    }*/
}

void MainWindow::receiveLog(QString msg)
{
    ui->statusBar->showMessage(msg, 2000);
    ui->txtOutput->appendPlainText(msg);
}

void MainWindow::receiveError(QString error)
{
    QMessageBox mb;
    mb.warning(this, "Error", error);
}

MainWindow::~MainWindow()
{
    delete ui;
}
