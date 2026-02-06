#include <GL/glut.h>

#include "../src/ball.h"
#include "../src/engine.h"
#include "../src/object.h"
#include "../src/quadtree.h"
vector<ball> otherBalls;
quadTree* original = new quadTree(1.0, -1.0, 1.0, -1.0);
void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  for (auto x : otherBalls) x.drawShape();

  glutSwapBuffers();
}
void update(int value) {
  // Keeps spawning balls up till 1000.
  if (otherBalls.size() < 8000) {
    otherBalls.push_back(ball());
    otherBalls.back().changePosition((rand() + 1.0) / (100.0 * RAND_MAX), 0);
  }
  // Alternating color
  glClear(GL_COLOR_BUFFER_BIT);
  original->clear();
  // Takes by reference and in linear time creates the quadtree.
  for (auto& x : otherBalls) {
    original->insert(&x);
  }
  // Applie's gravity to the balls.
  for (auto& x : otherBalls) x.applyGravity();
  // Applies the collisoin in nlog(n) time which is bvetter thhan the n^2
  for (int iter = 0; iter < 1; iter++) {
    original->collision();
  }
  // Updates position then draws.
  for (auto& x : otherBalls) x.updatePosition();
  for (auto& x : otherBalls) x.drawShape();
  // Alternates the frame.
  glutPostRedisplay();
  glutTimerFunc(16, update, 0);
}
bool running = true;  // controls the update loop

void handleKey(unsigned char key, int x, int y) {
  if (key == 27 || key == 'q' || key == 'Q') {
    running = false;
    delete original;
    exit(0);
  }
}
int main(int argc, char** argv) {
  original->isRootSet();

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutInitWindowSize(1000, 1000);
  glutCreateWindow("Particle sim");
  glutKeyboardFunc(handleKey);  // <-- add this
  glClearColor(0.0, 0.0, 0.0, 1.0);

  glutDisplayFunc(display);
  if (running) glutTimerFunc(1, update, 0);
  glutMainLoop();
  return 0;
}