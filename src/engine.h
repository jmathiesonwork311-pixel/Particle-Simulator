#ifndef ENGINE_H
#define ENGINE_H
#include <GL/glut.h>

#include "ball.h"
#include "quadtree.h"

class engine {
 public:
  static int totalBalls;
  static bool running;
  static vector<ball> otherBalls;
  static quadTree* original;
  static engine* current;

  engine(int argc, char** argv, int balls);
  static void draw();
  void handleKey(unsigned char key);
  static void endProgram();
  void update();
  void collision();
  ~engine();
};

#endif
