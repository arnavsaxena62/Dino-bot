#include "neuralnet.h"
#include <algorithm>
using namespace std;

NeuralNet::NeuralNet(GA1DArrayGenome<float>& g) : genome(g) {}

vector<float> NeuralNet::forward(vector<float> input) {
    int idx = 0;
    vector<float> hidden(HIDDENSIZE);
    for (int i = 0; i < HIDDENSIZE; i++) {
        float sum = genome.gene(idx++);
        for (int j = 0; j < INPUTSIZE; j++)
            sum += input[j] * genome.gene(idx++);
        hidden[i] = max(0.0f, sum);
    } 
    vector<float> output(OUTPUTSIZE);
    for (int i = 0; i < OUTPUTSIZE; i++) {
        float sum = genome.gene(idx++);
        for (int j = 0; j < HIDDENSIZE; j++)
            sum += hidden[j] * genome.gene(idx++);
        output[i] = sum;
    }
    return output;
}

int NeuralNet::getAction(vector<float> input) {
    vector<float> out = forward(input);
    return max_element(out.begin(), out.end()) - out.begin();
}
