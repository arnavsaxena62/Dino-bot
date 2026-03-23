#include "ga/GABaseGA.h"
#include "ga/GAGenome.h"
#include <algorithm>
#include <fstream>
#include <ga/GASimpleGA.h>
#include <iostream>
#include <vector>
#include <ga/GA1DArrayGenome.h>

#include "dino.h"
#include "neuralnet.h"
#include "constants.h"
using namespace std;

float objective(GAGenome& g) {
    GA1DArrayGenome<float>& genome = (GA1DArrayGenome<float>&)g;
    NeuralNet net(genome);

    bool alive = true;
    float score = 0;
    const int MAX_STEPS = 200000;  // cap
    dino player;                  
    vector<cactus> cactusArray;
    cactusArray.push_back(cactus());

    while (alive && score <= MAX_STEPS) {
        gamestep(net, cactusArray, player, alive);
        score++;
    }

    return score;
}

void initGenome(GAGenome& g, void*) {
    GA1DArrayGenome<float>& genome = (GA1DArrayGenome<float>&)g;
    for (int i = 0; i < genome.length(); i++)
        genome.gene(i, GARandomFloat(-1.0f, 1.0f));
}

void saveGenome(GASimpleGA ga){
  GA1DArrayGenome<float>& best = (GA1DArrayGenome<float>&)ga.statistics().bestIndividual();

  ofstream file("bestgenome.txt");
  for(int i = 0; i < best.length(); i++){
    file << best.gene(i) << "\n";
  }
  file.close();
  cout << "saved" << endl;
  
}

void trainGenome(); 

int main() {
  srand(time(nullptr));
    

  GA1DArrayGenome<float> genome(NUM_WEIGHTS, objective);
  ((GAGenome&)genome).initializer(initGenome);
  GASimpleGA ga(genome);
  ga.populationSize(100);
  ga.nGenerations(500);
  ga.pMutation(0.01);
  ga.pCrossover(0.6);

  ga.initialize();
  for (int gen = 0; gen < 500; gen++) {
      ga.step();
      cout << "Gen " << gen 
           << " Best: " << ga.statistics().bestIndividual().score()
           << endl;
  }

  cout << "Best score: " 
       << ga.statistics().bestIndividual().score() << endl;
  
  saveGenome(ga);
  return 0;
}
