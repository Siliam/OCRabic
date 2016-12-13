/*
 *  File    : KNNLearner.h
 *  Desc    : Implementing the K-NearestNeighbors Algorithm
 *  Author  : Ismail
 */

#ifndef KNNLEARNER_H
#define KNNLEARNER_H

#include <QObject>
#include <vector>
#include "image.h"

using namespace std;
typedef vector<double> vec;
typedef vector<vector<double>> vvec;

class KNNLearner : public QObject
{
    Q_OBJECT
private:
    int  _K;
    int  _nTotal;
    int  _nClasses;
    vvec _data;
    vec  _dataY;
    vvec* _avr;

public:
    explicit KNNLearner(int k = 2, QObject *parent = 0);

    void test();
    double eucDist(const vec& a, const vec& b);
    void load(QVector<Image*>* data, int dim);
    vector<int> getKNearstNeighbors(const vec& a, int k);

signals:

public slots:
};

#endif // KNNLEARNER_H
