/*
 *  File    : MainWindow.h
 *  Desc    : Standard Qt GUI unit. Handles UI and events
 *  Author  : Ismail
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cvworker.h"
#include "nnlearner.h"
#include "knnlearner.h"
#include <QMainWindow>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    CVWorker* worker;
    QStringList _labels;

    QVector<Image*>* _images;
    NNLearner*  _nn;
    KNNLearner* _knn;

    void setupUI();
    void setupConnections();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void sendError(QString error);
    void sendLog(QString message);
    void sendLoadImages(QString dirname);

public slots:
    void testNNLearner();
    void testKNNLearner();
    void receiveLoad();
    void receiveError(QString error);
    void receiveLog(QString message);
    void receiveLoadedImages(QVector<Image*>* images);
};

#endif // MAINWINDOW_H
