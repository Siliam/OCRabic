/*
 *  File    : KNNLearner.cpp
 *  Desc    :
 *  Author  : Ismail
 */

#include "knnlearner.h"
#include <cmath>
#include <ctime>
#include <algorithm>
#include <vector>
#include <QDebug>

using namespace std;

KNNLearner::KNNLearner(int k, QObject *parent) : QObject(parent), _K(k) {  }

void KNNLearner::load(QVector<Image*>* data, int dim)
{
    _nTotal = data->size();
    _data   = vvec(_nTotal, vec(dim));
    _dataY  = vec(_nTotal);

    srand(time(NULL));
    QVector<Image*> rdata(*data);
    random_shuffle(rdata.begin(), rdata.end());

    for(int i = 0; i < _nTotal; i++)
    {
        for(int j = 0; j < dim; j++)
            _data[i][j] = rdata[i]->val(j);
        _dataY[i] = rdata[i]->getClass();
    }

    qDebug() << "Training set size : " << _nTotal;
}

double KNNLearner::eucDist(const vec& a, const vec& b)
{
    double dist = 0.0;
    for(int i = 0; i < a.size(); i++) dist += pow(a[i] - b[i], 2);
    return sqrt(dist);
}

bool compareDists(pair<int, double> x, pair<int, double> y)
{
    return (x.second < y.second);
}

vector<int> KNNLearner::getKNearstNeighbors(const vec& a, int k)
{
    vector<pair<int, double> > distances(_nClasses);
    for(int i = 0; i < _nClasses; i++)
        distances[i] = {i, eucDist(a, (*_avr)[i])};
    sort(distances.begin(), distances.end(), compareDists);
    vector<int> results(k);

    for(int i = 0; i < k; i++)
        results[i] = distances[i].first;

    return results;
}

void KNNLearner::test()
{
    vector<int> v = getKNearstNeighbors(_data[0], 5);
    qDebug() << v;
}
