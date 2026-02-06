#include "quadtree.h"

#include <thread>
int quadTree::capacity = 20;
quadTree::quadTree(double right, double left, double top, double bot) {
  // creates the quadtree using the boundaries assigned.
  this->right = right;
  this->left = left;
  this->top = top;
  this->bot = bot;
  // Sets the smaller quadtrees to be smaller
  topLeft = nullptr;
  topRight = nullptr;
  botLeft = nullptr;
  botRight = nullptr;
}
void quadTree::divide() {
  // Divides borders based on curret borders.
  topLeft = new quadTree((right + left) / 2.0, left, top, (top + bot) / 2.0);
  topRight = new quadTree(right, (right + left) / 2.0, top, (top + bot) / 2.0);
  botLeft =
      new quadTree(right - (right - left) / 2.0, left, (bot + top) / 2.0, bot);
  botRight = new quadTree(right, (left + right) / 2.0, (top + bot) / 2.0, bot);
}

void quadTree::insert(ball* grain) {
  pair<double, double> pos = grain->getPosition();
  if (!(topLeft == nullptr)) {
    ball* x = grain;
    pos = x->getPosition();
    double radius = x->getRadius();
    // Insert top left if inside or bot left etc. Includes overlap
    if (pos.first - radius <= ((right + left)) / 2.0) {
      if (pos.second + radius >= (top + bot) / 2.0) topLeft->insert(x);
      if (pos.second - radius <= (top + bot) / 2.0) botLeft->insert(x);
    }
    if (pos.first + radius >= (right + left) / 2.0) {
      if (pos.second + radius >= (top + bot) / 2.0) topRight->insert(x);
      if (pos.second - radius <= (top + bot) / 2.0) botRight->insert(x);
    }
    return;
  }
  // If they aren't at capaciuty or is too small just don't increase size
  if ((capacity > (int)balls.size() || topLeft == nullptr) && right - left < 0.05) {
    balls.push_back(grain);
    return;
    // else if the quadtree has not subdivided
  } else if (topLeft == nullptr) {
    this->divide();
    // divide
    balls.push_back(grain);
    for (auto& x : balls) {
      // Inserts depending on the quadrants
      pos = x->getPosition();
      if (pos.first <= ((right + left)) / 2.0) {
        if (pos.second <= (top + bot) / 2.0)
          topLeft->insert(x);
        else
          botLeft->insert(x);
      } else {
        if (pos.second <= (top + bot) / 2.0)
          topRight->insert(x);
        else
          botRight->insert(x);
      }
    }
    // removes balls from upper quadrant
    balls.clear();
  }
}

void quadTree::clear() {
  if (topLeft != nullptr) topLeft->clear();
  if (topRight != nullptr) topRight->clear();
  if (botLeft != nullptr) botLeft->clear();
  if (botRight != nullptr) botRight->clear();
  balls.clear();
}
void quadTree::collision() {
  // Runs 4 threads on the first layer of the quadtree.
  if (topLeft != nullptr && isRoot) {
    thread t1([&]() { topLeft->collision(); });
    thread t2([&]() { topRight->collision(); });
    thread t3([&]() { botLeft->collision(); });
    thread t4([&]() { botRight->collision(); });
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    // Joins the threads toether
  } else if (topLeft != nullptr) {
    // Runs the collisions if it's not a nullptr
    topLeft->collision();
    topRight->collision();
    botLeft->collision();
    botRight->collision();
    return;
  }
  // checks for all balls that aren't itself
  for (int i = 0; i < (int)balls.size(); i++) {
    for (int j = i + 1; j < (int)balls.size(); j++) {
      ball *x = balls[j], *y = balls[i];
      // Position overlap, getting variables to work with
      pair<double, double> xPos = x->getPosition(), yPos = y->getPosition();
      pair<double, double> xVel = x->getVelocity(), yVel = y->getVelocity();
      double d_x = x->getPosition().first - y->getPosition().first;
      double d_y = x->getPosition().second - y->getPosition().second;
      if (d_x * d_x + d_y * d_y > (x->getRadius() + y->getRadius()) *
                                      (x->getRadius() + y->getRadius())) {
        continue;
      }
      // Incase balls are same position offset them
      double dist = sqrt(d_x * d_x + d_y * d_y);
      if (dist < 0.00001) dist += 0.00001;

      double overlap = x->getRadius() + y->getRadius() - dist;
      vector<double> directionVector = {d_x / dist, d_y / dist};
      double velocityVal = (y->getVelocity().second - x->getVelocity().second) *
                               directionVector[1] +
                           (y->getVelocity().first - x->getVelocity().first) *
                               directionVector[0];
      vector<double> overlapCorrection = {directionVector[0] * overlap * 0.5,
                                          directionVector[1] * overlap * 0.5};
      // Fixes overlap.
      if (0 < overlap) {
        if (yPos.second < xPos.second) {
          if (xVel.second < 0) xVel.second *= 0.90;
        } else if (yPos.second > xPos.second) {
          if (yVel.second < 0) yVel.second *= 0.90;
        }
        yPos.first -= overlapCorrection[0];
        yPos.second -= overlapCorrection[1];
        xPos.first += overlapCorrection[0];
        xPos.second += overlapCorrection[1];
        yVel.first -= directionVector[0] * velocityVal * 1.3;
        yVel.second -= directionVector[1] * velocityVal;
        xVel.first += directionVector[0] * velocityVal * 1.3;
        xVel.second += directionVector[1] * velocityVal;

        if (yVel.second < 0 && xPos.second < yPos.second) yVel.second *= 0.995;
      }
      // Trying to lower the spread when tuching the ground
      if (yPos.second <= y->getRadius() + 0.001) {
        yVel.first *= 0.1;
      }
      // lowers the horizontal spread more
      xVel.first *= 0.5;
      yVel.first *= 0.5;

      x->setPos(xPos);
      y->setPos(yPos);
      x->setVel(xVel);
      y->setVel(yVel);
    }
  }
}
quadTree::~quadTree() {
  // clears the quadtree.
  delete (topLeft);
  delete (topRight);
  delete (botRight);
  delete (botLeft);
  topLeft = nullptr;
  topRight = nullptr;
  botLeft = nullptr;
  botRight = nullptr;
  balls.clear();
}
void quadTree::isRootSet() { isRoot = true; }