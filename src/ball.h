#ifndef BALL_H
#define BALL_H

#include <cmath>

#include "maths.h"
#include "object.h"
class ball : public object {
  double red = 1;
  double green = max((rand() / (1.3 * RAND_MAX)), 0.5);
  // Information.
  double radius = max(rand() / (100 * (RAND_MAX + 0.0)), 0.001);
  // Static variables for ID, total balls, and to search for collisions.
  static long long totalBalls;
  static long long ballId;
  static std::vector<ball*> otherBalls;
  static std::vector<ball*> allBalls;
  unsigned int VAO = 0;
  unsigned int VBO = 0;
  long long id;
  pair<double, double> force = {0, 0};
  pair<double, double> velocity = {0, 0};
  pair<double, double> position = {0, 0};

 public:
  // Getters
  pair<double, double> getPosition() const override;
  double getRadius();
  ball();
  // Detect Collision
  int getBallId();
  void getVectorSize();
  void changePosition(double x, double y);
  void clearForce();
  void collision() override;
  void updateVelocity();
  // Change shapre properties
  void drawShape() const override;
  void deleteShape();
  void moveObject() override;
  void changeObjectType() override;
  void changeObjectColorTotality() const override;
  void changeObjectColorContinuing() const override;
  void applyGravity();
  void generateBall();
  void setVel(pair<double, double> velocity);
  void setPos(pair<double, double> position);
  pair<double, double> getVelocity();
  ~ball();
  void updatePosition();
};
#endif