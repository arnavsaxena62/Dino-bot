
#include "dino.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>
using namespace std;



void spawnCactus(vector<cactus>& cactusArray) {
  bool shouldSpawn = cactusArray.empty() ||
                     cactusArray.back().x <= SCREENW - MIN_GAP;
  
  if (!shouldSpawn) return;

  cactus c;
  c.x = SCREENW + (rand() % (MAX_GAP - MIN_GAP) + MIN_GAP);
  c.y = 0;
  c.w = 30;
  c.h = 60;
  c.velocity = BASE_VELOCITY;
  c.active = true;
  cactusArray.push_back(c);

}

void gamestep(NeuralNet net, vector<cactus>& cactusArray, dino& player, bool& running){
  if (cactusArray.empty()) { running = false; return; }

  for(auto& i : cactusArray) {
    if (i.active) {
      i.step();
      if (player*i) {
        running = false;
      } 
    }
  }

  cactusArray.erase(
    remove_if(cactusArray.begin(), cactusArray.end(), 
              [](const cactus& c){ return !c.active; }),
    cactusArray.end()
  );
 

  spawnCactus(cactusArray);
  
  float distanceFromCactus = cactusArray[0].x - player.x;

  
  int action = net.getAction((vector<float>){distanceFromCactus});

  if(action == 1){ player.jump(); }
  player.step();
  

}

