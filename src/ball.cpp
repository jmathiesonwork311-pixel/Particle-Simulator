#include "ball.h"
// Static variables, total balls,
long long ball::totalBalls = 0;
long long ball::ballId = 0;
vector<ball*> ball::allBalls;
vector<ball*> ball::otherBalls;
#define PI 3.146;

void ball::getVectorSize() { cout << ball::otherBalls.size() << endl; }
void ball::clearForce() { force = {0, 0}; }
void ball::applyGravity() { velocity.second -= 0.8 * 0.016; }
void ball::changeObjectType() { return; }
void ball::changeObjectColorContinuing() const {}
void ball::changeObjectColorTotality() const {}
void ball::changePosition(double x, double y) { position = {x, y}; }

void ball::collision() {
  // control force increaes
  for (int i = id; i < (int)otherBalls.size(); i++) {
    ball* x = otherBalls[i];
    if (x->id != id) {
      double d_x = x->position.first - position.first;
      double d_y = x->position.second - position.second;
      double dist = sqrt(d_x * d_x + d_y * d_y);
      if (dist < 0.00001) dist += 0.00001;

      double overlap = x->radius + radius - dist;
      vector<double> directionVector = {d_x / dist, d_y / dist};
      double velocityVal =
          (velocity.second - x->velocity.second) * directionVector[1] +
          (velocity.first - x->velocity.first) * directionVector[0];
      vector<double> overlapCorrection = {directionVector[0] * overlap * 0.1,
                                          directionVector[1] * overlap * 0.9};

      if (0 < overlap) {
        position.first -= overlapCorrection[0];
        position.second -= overlapCorrection[1];
        x->position.first += overlapCorrection[0];
        x->position.second += overlapCorrection[1];
        velocity.first -= directionVector[0] * velocityVal * 0.1;
        velocity.second -= directionVector[1] * velocityVal * 0.5;
        x->velocity.first += directionVector[0] * velocityVal * 0.1;
        x->velocity.second += directionVector[1] * velocityVal * 0.5;

        if (velocity.second < 0 && x->position.second < position.second)
          velocity.second *= 0.995;
      }
      if (position.second <= radius + 0.001) {
        velocity.first *= 0.1;  // reduce horizontal sliding
      }
    }
  }
  // Learned from OPENGL page
  // https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-detection
  return;
}
void ball::updatePosition() {
  // Position update per 16 milliseconds.
  position.first += velocity.first * 0.016;
  position.second += velocity.second * 0.016;
  // If  lower than floor, correct psotion and velocity/apply friction.
  if (position.second < radius - 1) {
    position.second = -1.0 + radius, velocity.second *= -0.1,
    velocity.first *= 0.03;
  } else if (position.second > 1 - radius)
    // Ceiling checker.
    velocity.first *= 0.0001, position.second = -1.0 + radius;
  if (position.first > -radius + 1) {
    position.first = 1.0 - radius;
    velocity.first *= -0.1;
  } else if (position.first < radius - 1)
    position.first = radius - 1.0, velocity.first *= -0.1;

  // https://en.wikipedia.org/wiki/Elastic_collision#Two-dimensional
}

int ball::getBallId() { return id; }
void ball::updateVelocity() {
  if (abs(velocity.first) < abs((0.001))) velocity.first = 0;
  if (abs(velocity.second) < abs(0.01)) velocity.second = 0;
  if (position.second < radius - 1)
    position.second = -1.0 + radius, velocity.second *= -0.1;
  else if (position.second > 1 - radius)
    position.second = -1.0 + radius, velocity.second *= -0.1;
  if (position.first > -radius + 1) {
    position.first = 1.0 - radius;
    velocity.first *= -0.1;
  } else if (position.first < radius - 1)
    position.first = radius - 1.0, velocity.first *= -0.1;
}
// https://en.wikipedia.org/wiki/Elastic_collision#Two-dimensional

ball::ball() {
  // Declare position. Then velcity.
  position = {0, 0};
  velocity = {0, 0};
  // Increment static values.
  id = ballId++;
  totalBalls++;
  ball::otherBalls.push_back(this);
}
void ball::generateBall() {}
void ball::deleteShape() {}
void ball::drawShape() const {
  // Changing GL_ to GL_POLYGON to create a solid circle
  glBegin(GL_POLYGON);
  // Randomised colour to add noise to the appearnace.
  glColor3f(red, green, 0);
  // More ore less lines to increase speed
  int amountOfLines = 16;

  for (int i = 0; i < amountOfLines; i++) {
    // Calculate the angle for this point
    double angle = 2.0f * 3.1415926f * i / amountOfLines;

    // Calculate x and y based on the angle
    double x = radius * cosf(angle) + position.first;
    double y = radius * sinf(angle) + position.second;

    glVertex2f(x, y);
  }

  glEnd();
}
std::pair<double, double> ball::getPosition() const { return position; }
std::pair<double, double> ball::getVelocity() { return velocity; }
double ball::getRadius() { return radius; }
void ball::moveObject() {
  position.first += velocity.first;
  position.second += velocity.second;
}

void ball::setVel(pair<double, double> velocity) {
  this->velocity.first = velocity.first;
  this->velocity.second = velocity.second;
}
void ball::setPos(pair<double, double> position) {
  this->position.first = position.first;
  this->position.second = position.second;
}

ball::~ball() { deleteShape(); }