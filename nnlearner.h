/*
 *  File    : NNLearner.h
 *  Desc    : Implementing a single hidden layer neural network
 *  Author  : Ismail
 */

#ifndef NNLEARNER_H
#define NNLEARNER_H

#include <Vector>
#include <cmath>
#include "Image.h"

using namespace std;
typedef vector<double> vec;
typedef vector<vector<double>> vvec;

// Let's start with single layer neural network, multilayer coming up soon !
class NNLearner
{
private :
    int _in, _out;
    int _size;
    double _learningRate;
    int _batchSize, _nEpochs;
    int _nTotal, _nTrain;
    float _testRatio;
    int _method;

    vvec _W1, _W2;
    vec _z1, _z2;
    vec _a1, _a2;
    vec _d1, _d2;

    vvec _dataTrain;
    vvec _dataTest;
    vec _dataY;

public:
    NNLearner(int in, int out,          // input and output layer dimensions
              int size,                 // soon to become "QVector<int> sizes" for multilayer
              double learningRate,      // How "fast" learning is (add special values for adaptive learningRate ?)
              int batchSize = 10,       // if set to negative then it's the whole dataset
              int nEpochs = 3);         // TODO

    void load(QVector<Image*>* data,         // the vector containing all training images and their classes
              float testRatio,          // how much of the data is left for testing
              int method = 0);          // how to pick the data to train on

    double trainBatch(int k);           // update the weight using a batch from SGD

    void stochasticGD();
    void feedforward(const vec& a);
    void backpropopagation(int k, vvec& dW1, vvec& dW2, double& error);

    void cost_derivative();

    double sigmoid(double z);
    double dsigmoid(double z);

    void test();

    ~NNLearner();
};

#endif // NNLEARNER_H
