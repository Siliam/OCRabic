/*
 *  File    : CVWorker.cpp
 *  Desc    :
 *  Author  : Ismail
 */

#include "cvworker.h"
#include <QtGui>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


bool getClassAndIndex(QString s, int& c, int& i);

CVWorker::CVWorker(QObject *parent) : QObject(parent) { }

void CVWorker::receiveLoadImages(QString dirname)
{
    QFileInfoList files = QDir(dirname).entryInfoList();
    emit sendLog("Loading data ..");

    QTime myTimer;
    myTimer.start();

    for(int i = 0; i < _limit && i < files.size(); i++)
    {
        if(files[i].isDir())
            continue;

        QString filename = files[i].filePath();
        cv::Mat mat;
        int n, c;

        try{
            if(getClassAndIndex(files[i].fileName(), c, n))
            {
                mat = cv::imread(filename.toStdString());
                // cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
                cv::cvtColor(mat, mat, cv::COLOR_BGR2GRAY);
            }
            else
                continue;
        }
        catch (...) {
            images.clear();
            emit sendError("Cannot open : " + filename);
            return;
        }

        images.push_back(new Image(mat.clone(), c, n));
    }

    long timeElapsed = myTimer.elapsed();

    if(images.size() == 0)
        emit sendError("Error loading data !");

    emit sendLog(QString("Time to load %1 files : ~%2 ms").arg(images.size()).arg(timeElapsed));
    emit sendLoadedImages(&images);
}

CVWorker::~CVWorker()
{
    images.clear();
}

bool getClassAndIndex(QString s, int& c, int& i)
{
    try
    {
        QStringList l = s.split(".")[0].split("_");
        if(l.size() < 2) return false;
        c = l[0].toInt();
        i = l[1].toInt();
        return true;
    }
    catch (...)
    {
        return false;
    }
}
