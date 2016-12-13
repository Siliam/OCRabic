/*
 *  File    : NNLearner.cpp
 *  Desc    :
 *  Author  : Ismail
 */

#include "nnlearner.h"
#include <QDebug>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <iomanip>

using namespace std;

double nrand();
void printvVec(vvec& v, string s = "");
void printVec(vec& v, string s = "");

NNLearner::NNLearner(int in, int out, int size, double learningRate, int batchSize, int nEpochs) :
    _W1(size, vec(in + 1)), _W2(out, vec(size + 1)),
    _z1(size), _z2(out),
    _a1(size), _a2(out),
    _d1(size), _d2(out)
{
    srand(time(NULL));
    float e1 = sqrt(6) / sqrt(in + size);
    float e2 = sqrt(6) / sqrt(size + out);

    for(int i = 0; i < _W1.size(); i++)
        for(int j = 0; j < _W1[0].size(); j++)
            _W1[i][j] = nrand() * 2 * e1 - e1;

    for(int i = 0; i < _W2.size(); i++)
        for(int j = 0; j < _W2[0].size(); j++)
            _W2[i][j] = nrand() * 2 * e2 - e2;

    _in = in;
    _out = out;
    _size = size;
    _learningRate = learningRate;
    _batchSize = batchSize;
    _nEpochs = nEpochs;
}


void NNLearner::load(QVector<Image*>* data, float testRatio, int method)
{
    _nTotal = data->size();
    _nTrain = (int) ((1 - testRatio)*_nTotal);
    _testRatio = testRatio;
    _method = method;

    _dataTrain = vvec(_nTrain, vec(_in));
    _dataTest  = vvec(_nTotal - _nTrain, vec(_in));
    _dataY     = vec(_nTotal);

    QVector<Image*> rdata(*data);
    random_shuffle(rdata.begin(), rdata.end());

    for(int i = 0; i < _nTotal; i++)
    {
        for(int j = 0; j < _in; j++)
            if(i < _nTrain)
                _dataTrain[i][j] = rdata[i]->val(j);
            else
                _dataTest[i - _nTrain][j] = rdata[i]->val(j);
        _dataY[i] = rdata[i]->getClass();
    }
    qDebug() << "Training set size : " << _nTrain;
}


void NNLearner::feedforward(const vec& x)
{
    // Input to Hidden
    for(int i = 0; i < _size; i++)
    {
        _z1[i] = 0;
        for(int j = 0; j < _in + 1; j++)
            _z1[i] += (j == 0) ? (_W1[i][j]) : (_W1[i][j] * x[j-1]);
        _a1[i] = sigmoid(_z1[i]);
    }

    // Hidden to Output
    for(int i = 0; i < _out; i++)
    {
        _z2[i] = 0;
        for(int j = 0; j < _size + 1; j++)
            _z2[i] += (j == 0) ? (_W2[i][j]) : (_W2[i][j] * _a1[j-1]);
        _a2[i] = sigmoid(_z2[i]);
    }
}


void NNLearner::backpropopagation(int k, vvec& dW1, vvec& dW2, double& error)
{
    error = 0.0;
    auto x = _dataTrain[k];
    feedforward(x);

    double i_max_a = 0;
    for(int i = 0; i < _out; i++)
    {
        _d2[i] = _a2[i] - (_dataY[k] == i);
        if(_a2[i] > _a2[i_max_a]) i_max_a = i;
    }
    // Count misclassified examples
    error += _dataY[k] != i_max_a;

    for(int i = 0; i < _size; i++)
    {
        _d1[i] = 0;
        for(int j = 0; j < _out; j++)
            _d1[i] += _a2[j]*_W2[j][i+1];
        _d1[i] = _d1[i] * dsigmoid(_z1[i]);
    }

    // Backpropagate the error
    for(int i = 0; i < _size; i++)
        for(int j = 0; j < _in + 1; j++)
            dW1[i][j] += (j == 0) ? _d1[i] : x[j - 1]*_d1[i];

    for(int i = 0; i < _out; i++)
        for(int j = 0; j < _size + 1; j++)
            dW2[i][j] += (j == 0) ? _d2[i] : _a1[j - 1]*_d2[i];
}


double NNLearner::trainBatch(int k)
{
    vvec dW1(_size, vec(_in + 1, 0));
    vvec dW2(_out, vec(_size + 1, 0));

    double error, totalError = 0;

    for(int i = k*_batchSize; i < (k + 1)*_batchSize; i++)
    {
        backpropopagation(i, dW1, dW2, error);
        totalError += error;
    }

    for(int i = 0; i < _size; i++)
        for(int j = 0; j < _in + 1; j++)
            _W1[i][j] -= (_learningRate/_batchSize) * dW1[i][j];

    for(int i = 0; i < _out; i++)
        for(int j = 0; j < _size + 1; j++)
            _W2[i][j] -= (_learningRate/_batchSize) * dW2[i][j];

    return totalError;
}


void NNLearner::stochasticGD()
{
    qDebug() << "< STARTING STOCHASTIC GRADIENT DESCENT >" << endl;

    int nBatches = _dataTrain.size()/_batchSize;
    for(int i = 0; i < _nEpochs; i++)
    {
        double totalError = 0.0;
        for(int k = 0; k < nBatches; k++)
        {
            double error = trainBatch(k);
            totalError += error;
            qDebug() << "Iteration " << k + 1 << "/" << nBatches << " in Epoch " << i + 1 << "  \t(Error : " << error << ")";
        }
        qDebug() << "Epoch " << i + 1 << "/" << _nEpochs << " DONE.  (Total error : " << (int) totalError << ")";
    }

    qDebug() << "Training done !";
    /*
    printvVec(_W1, "W1");
    printvVec(_W2, "W2");
    */
}


double NNLearner::sigmoid(double z)
{
    return 1.0/(1.0 + exp(-z));
}


double NNLearner::dsigmoid(double z)
{
    return sigmoid(z) * (1 - sigmoid(z));
}


void NNLearner::test()
{
    stochasticGD();

    /*
    qDebug() << "x: " << _dataTrain[0];
    qDebug() << "a1: " << _a1;
    qDebug() << "z1: " << _z1;
    qDebug() << "W1: " << _W1[0];

    feedforward(_dataTrain[0]);

    qDebug() << " AFTER ! ";

    qDebug() << "a1: " << _a1;
    qDebug() << "z1: " << _z1;

    Image dt0 = Image(_dataTrain[0], 32, 16);
    dt0.show("test !");
    */

}

NNLearner::~NNLearner()
{
}

//
// --- Helper Functions ---
//
double nrand()
{
    return ((double) rand()) / RAND_MAX;
}

void printvVec(vvec& v, string s)
{
    if(s.size())
        cout << "vVec [" << s << "]:\n";

    for(int i = 0; i < v.size(); i++)
    {
        cout << "[ ";
        for(int j = 0; j < v[0].size(); j++)
            cout << setprecision(2) << v[i][j] <<  " ";
        cout << "]" << endl;
    }
}

void printVec(vec& v, string s)
{
    if(s.size())
        cout << "vec [" << s << "]:\n";

    cout << "[ ";
    for(int i = 0; i < v.size(); i++)
        cout << setprecision(2) << v[i] <<  " ";
    cout << "]" << endl;
}
