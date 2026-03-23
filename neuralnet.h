#ifndef NEURALNET_H
#define NEURALNET_H

#include <vector>
#include <ga/GA1DArrayGenome.h>
#include "constants.h"
using namespace std;

class NeuralNet {
public:
    GA1DArrayGenome<float>& genome;
    NeuralNet(GA1DArrayGenome<float>& g);
    vector<float> forward(vector<float> input);
    int getAction(vector<float> input);
};

#endif
