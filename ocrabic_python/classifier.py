import csv
import random
import numpy as np
from sklearn import svm
from sklearn.neural_network import MLPClassifier
from sklearn.preprocessing import normalize
from matplotlib import pyplot as plt
from math import ceil

classifiers = [ ('linear', svm.SVC(kernel='linear', C=1e3)),
                ('poly',   svm.SVC(kernel='poly', C=1e3, degree= 3)),
                ('rbf',    svm.SVC(kernel='rbf', C=1, gamma= 0.04)),
                ('linSVC', svm.LinearSVC(C=1e-2, tol=0.2)),
                ('NN',     MLPClassifier(solver='lbfgs', activation='logistic', alpha=1e-5, hidden_layer_sizes=(100, 3))) ]

filename = "data/alef.csv"
trr = 0.8  #trainset ratio

def load_csv():
    vec = []
    f =  open(filename, 'r')
    csv_r = csv.reader(f)
    for row in csv_r:
        vec.append(map(int,row))
    f.close()
    return vec

def load_data():
    X = load_csv()
    n_examples = len(X)
    npc = n_examples / 2    # number of examples per class
    Y = [0] * npc + [1] * npc
    
    n_train = int(ceil(npc*trr))
    Xtrain = np.array(X[0:n_train] + X[npc:npc + n_train]) / 255.0
    Ytrain = np.array(Y[0:n_train] + Y[npc:npc + n_train])
    
    Xtest = np.array(X[n_train:npc] + X[npc + n_train:]) / 255.0
    Ytest = np.array(Y[n_train:npc] + Y[npc + n_train:])
    
    return Xtrain, Ytrain, Xtest, Ytest, n_examples
    
def visualize_one(x):
    plt.axis('off')
    plt.imshow(x.reshape((32,16)), cmap='gray', vmin=0, vmax=1)
    plt.show()
    
def visualize(X, n_items_x, n_items_y):
    n_items = n_items_x * n_items_y
    sample = random.sample(X, n_items)
    
    for i, s in enumerate(sample):
        plt.subplot(n_items_x, n_items_y, i+1)
        plt.axis('off')
        plt.imshow(s.reshape((32,16)), cmap='gray', vmin=0, vmax=1)
    
    plt.show()
    
X, Y, Xt, Yt, n = load_data()

# Visualize data from both classes
# visualize(X[:n*trr/2], 3, 4)
# visualize(X[n*trr/2:], 3, 4)

for clf in classifiers : 
    classifier = clf[1]
    classifier.fit(X, Y)
    
    Pt = classifier.predict(Xt)
    correct = len([1 for p, y in zip(Pt, Yt) if p == y])
    total   = len(Yt)
    
    print clf[0] + ":\t " + str(correct) + ' correct predictions out of ' + str(total),
    print '(' + str(correct*100.0/total) + '%)'
