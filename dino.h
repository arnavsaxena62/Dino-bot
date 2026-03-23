#ifndef DINO_H
#define DINO_H

#include <vector>
#include "neuralnet.h"

#define GRAV       0.5f
#define JUMPFORCE  12.0f
#define SCREENW    800
#define SCREENH    400
#define MIN_GAP    300
#define MAX_GAP    600
#define BASE_VELOCITY 5.0f

using namespace std;

class collision_object{

public:
  float w,h,y,x;
  bool operator*(collision_object obj){
    float left1   = x;  
    float right1  = x + w;
    float bottom1 = y;
    float top1    = y + h;

    float left2   = obj.x;
    float right2  = obj.x + obj.w;
    float bottom2 = obj.y;
    float top2    = obj.y + obj.h;

    if (right1 < left2 || right2 < left1) return false;
    if (top1 < bottom2 || top2 < bottom1) return false;


    return true;

}
};

class dino : public collision_object{
  public:
  float vy = 0;
  bool grounded = true;
  

  dino(){
    x = 20.0f;
    y = 0;
    w = 15;
    h = 60;
  }

  void jump(){
    if (grounded) {
      vy = JUMPFORCE;
      grounded = false;
    }
  }
  


  void step(){
    vy-=GRAV;
    y+=vy;

    if(y<=0){
      y = 0;
      vy = 0;
      grounded = true;
    }

    if(y>0){
      grounded = false;
    }
  }

};

class cactus : public collision_object{

public:
  float velocity;
  bool active;

  void step(){
    x -= velocity;
    if(x <= -w){ this->active = false; }
  }

  cactus(){
    x = SCREENW;
    y = 0;
    w = 30;
    h = 60;
  }
  
};


void spawnCactus(vector<cactus>& cactusArray);
void gamestep(NeuralNet net, vector<cactus>& cactusArray, dino& player, bool& running);

#endif
