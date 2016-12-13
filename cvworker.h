/*
 *  File    : CVWorker.h
 *  Desc    : handles loading examples and transfering them to the MainWindow class
 *  Author  : Ismail
 */

#ifndef CVWORKER_H
#define CVWORKER_H

#include "Image.h"
#include <QObject>
#include <QVector>

class CVWorker : public QObject
{
    Q_OBJECT

private:
    QVector<Image*> images;

public:
    explicit CVWorker(QObject *parent = 0);
    ~CVWorker();

    int _limit = 20000; // to limit the number of images to load from the given folder

signals:
    void sendLoadedImages(QVector<Image*>* images);
    void sendError(QString error);
    void sendLog(QString error);

public slots:
    void receiveLoadImages(QString dirname);
};

#endif // CVWORKER_H
