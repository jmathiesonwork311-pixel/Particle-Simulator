#ifndef QUADTREE_H
#define QUADTREE_H
#include "ball.h"

class quadTree {
  static int capacity;
  bool isRoot = false;
  double left, right, top, bot;
  vector<ball*> balls;
  //Different quadTree quadrants
  quadTree* topLeft;
  quadTree* topRight;
  quadTree* botLeft;
  quadTree* botRight;

 public:
  void isRootSet();
  quadTree(double right, double left, double top, double bot);
  void divide();
  void insert(ball* grain);
  void collision();
  void clear();
  ~quadTree();
};

#endif