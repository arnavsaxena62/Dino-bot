#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "ga/GAGenome.h"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <ga/GA1DArrayGenome.h>
#include <ga/GASimpleGA.h>
#include <iostream>
#include <ostream>
#include <random>
#include <vector>

#include "constants.h"
#include "dino.h"
#include "neuralnet.h"
using namespace std;

float objective(GAGenome &g) {
  GA1DArrayGenome<float> &genome = (GA1DArrayGenome<float> &)g;
  NeuralNet net(genome);

  bool alive = true;
  float score = 0;
  const int MAX_STEPS = 200000; // cap
  dino player;
  vector<cactus> cactusArray;
  cactusArray.push_back(cactus());

  while (alive && score <= MAX_STEPS) {
    gamestep(net, cactusArray, player, alive);
    score++;
  }

  return score;
}

void initGenome(GAGenome &g, void *) {
  GA1DArrayGenome<float> &genome = (GA1DArrayGenome<float> &)g;
  for (int i = 0; i < genome.length(); i++)
    genome.gene(i, GARandomFloat(-1.0f, 1.0f));
}

void saveGenome(GASimpleGA ga) {
  GA1DArrayGenome<float> &best =
      (GA1DArrayGenome<float> &)ga.statistics().bestIndividual();

  ofstream file("bestgenome.txt");
  for (int i = 0; i < best.length(); i++) {
    file << best.gene(i) << "\n";
  }
  file.close();
  cout << "saved" << endl;
}

GA1DArrayGenome<float> loadGenome(const string filename) {
  GA1DArrayGenome<float> genome(NUM_WEIGHTS, objective);
  ifstream file(filename);
  for (int i = 0; i < NUM_WEIGHTS; i++) {
    float val;
    file >> val;
    genome.gene(i, val);
  }

  file.close();
  return genome;
}

int main(int argc, char *argv[]) {
  srand(time(nullptr));

  if (argc > 1 && string(argv[1]) == "--train") {

    GA1DArrayGenome<float> genome(NUM_WEIGHTS, objective);
    ((GAGenome &)genome).initializer(initGenome);
    GASimpleGA ga(genome);
    ga.populationSize(100);
    ga.nGenerations(500);
    ga.pMutation(0.01);
    ga.pCrossover(0.6);

    ga.initialize();

    for (int gen = 0; gen < 500; gen++) {
      ga.step();
      cout << "Gen " << gen
           << " Best: " << ga.statistics().bestIndividual().score() << endl;
    }

    cout << "Best score: " << ga.statistics().bestIndividual().score() << endl;

    saveGenome(ga);

  } else {

    sf::RenderWindow window(sf::VideoMode(800, 400), "Dino Bot Game");
    window.setFramerateLimit(60);
    cout << "screen on" << endl;

    GA1DArrayGenome<float> genome = loadGenome("best_genome.txt");
    NeuralNet net(&genome);

    bool alive = true;
    dino player;
    vector<cactus> cactusArray;

    spawnCactus(cactusArray);

    while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
          window.close();
      }

      gamestep(net, cactusArray, player, alive);

      bool collided = false;
      for (auto &c : cactusArray) {
        if (!c.active)
          continue;
        if (player * c) {
          collided = true;
          break;
        }
      }
      if (collided) {
        window.close();
      }

      window.clear(sf::Color::White);

      cout << "cactus count" << cactusArray.size() << endl;
      cout << cactusArray.empty() << endl;
      cout << "dino" << player.x << "," << player.y << endl;
      for (auto &c : cactusArray) {
        cout << "cactus x: " << c.x << " active: " << c.active << " y: " << c.y
             << endl;
      }

      player.draw(window);

      for (auto &c : cactusArray) {
        if (!c.active)
          continue;

        sf::RectangleShape cactusShape(sf::Vector2f(c.w, c.h));
        cactusShape.setPosition(c.x, c.y);
        cactusShape.setFillColor(sf::Color::Black);

        window.draw(cactusShape);
      }
      window.display();
    }
  }

  return 0;
}
