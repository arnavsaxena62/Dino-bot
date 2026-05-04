#ifndef DINO_H
#define DINO_H

#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "neuralnet.h"
#include <SFML/Graphics.hpp>
#include <vector>

#define GRAV 0.5f
#define JUMPFORCE 12.0f
#define SCREENW 800
#define SCREENH 400
#define MIN_GAP 300
#define MAX_GAP 600
#define BASE_VELOCITY 5.0f
#define GROUND 300

using namespace std;

class collision_object {

public:
  float w, h, y, x;
  int frame;
  float animationTimer;
  float animationSpeed;

  bool operator*(collision_object obj) {
    float left1 = x;
    float right1 = x + w;
    float bottom1 = y;
    float top1 = y + h;

    float left2 = obj.x;
    float right2 = obj.x + obj.w;
    float bottom2 = obj.y;
    float top2 = obj.y + obj.h;

    if (right1 < left2 || right2 < left1)
      return false;
    if (top1 < bottom2 || top2 < bottom1)
      return false;

    return true;
  }
};

class dino : public collision_object {
public:
  float vy = 0;
  bool grounded = true;

  sf::Texture texture;
  sf::Sprite sprite;

  dino() {
    x = 20.0f;
    y = GROUND;
    w = 40;
    h = 40;
    vy = 0;

    frame = 0;
    animationTimer = 0.0f;
    animationSpeed = 0.1f;

    texture.loadFromFile("dino.png");
    sprite.setTexture(texture);

    sprite.setScale(2.0f, 2.0f);
  };

  void jump() {
    if (grounded) {
      vy = JUMPFORCE;
      grounded = false;
    }
    cout << "jumped"<< endl;
  }

  void step() {
    vy -= GRAV;
    y -= vy;

    if (y >= GROUND) {
      y = GROUND;
      vy = 0;
      grounded = true;
    } else {
      grounded = false;
    }

    animationTimer += 0.016f;

    if (animationTimer >= animationSpeed) {
      animationTimer = 0.0f;
      frame = (frame + 1) % 4;
    }
  }

  void draw(sf::RenderWindow &window) {
    sprite.setTextureRect(sf::IntRect(frame * 24, 0, 24, 24));

    sprite.setPosition(x, y);
    window.draw(sprite);
  }
};

class cactus : public collision_object {

public:
  float velocity;
  bool active;

  void step() {
    x -= velocity;
    if (x <= -w) {
      this->active = false;
    }
  }

  cactus() {
    x = SCREENW;
    y = GROUND;
    w = 20;
    h = 40;
    active = true;
    velocity = BASE_VELOCITY;
  }
};

void spawnCactus(vector<cactus> &cactusArray);
void gamestep(NeuralNet net, vector<cactus> &cactusArray, dino &player,
              bool &running);

#endif
