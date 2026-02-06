    #include "engine.h"
    #include "quadtree.h"

    //Forard declaration
    void handleKey(char key, int x, int y);
    void updateHold(int val);
    void keyHandle(unsigned char key, int x, int y);
    void display();
    //Declares static var
    //declare quadtree to be size of display
    quadTree* engine::original = new quadTree(1.0, -1.0, 1.0, -1.0);
    engine* engine::current = nullptr;
    int engine::totalBalls = 0;
    vector<ball> engine::otherBalls = {};
    //argument list
    engine::engine(int argc, char** argv, int balls) {
        //sets root
    original->isRootSet();
    //makes sure it's just one engine
    engine::current = this;
    //totalballs
    totalBalls = balls;
    //Using argument count and vectors
    glutInit(&argc, argv);
    //Double frames to avoid jittering
    glutInitDisplayMode(GLUT_DOUBLE);
    //1000,100
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("Particle sim!");
    //Sets key check for indo exit
    glutKeyboardFunc(keyHandle);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glutDisplayFunc(display);
    glutTimerFunc(16, updateHold, 0);
glutMainLoop();
    }
    void display() {
        glClear(GL_COLOR_BUFFER_BIT);
        engine::draw();
        glutSwapBuffers();
    }
    void update(int value) {
    // Keeps spawning balls up till my limit.
    int size = engine::otherBalls.size();
    if (  size < (int)engine::totalBalls) {
        engine::otherBalls.push_back(ball());
        engine::otherBalls.back().changePosition(
            (rand() + 1.0) / (100.0 * RAND_MAX), 0);
    }
    // Alternating color
    engine::original->clear();
    // Takes by reference and in linear time creates the quadtree.
    for (auto& x : engine::otherBalls) {
        engine::original->insert(&x);
    }
    // Applies gravity to the balls.
    for (auto& x : engine::otherBalls) x.applyGravity();
    //Increasing the loop can improve realism.
    for (int iter = 0; iter < 1; iter++) {
        // Applies the collisoin in nlog(n) time which is bvetter thhan the n^2
        engine::original->collision();
    }
    // Updates position then draws.
    for (auto& x : engine::otherBalls) x.updatePosition();
    // Alternates the frame.
    glutPostRedisplay();
    glutTimerFunc(16, updateHold, 0);
    }
    void keyHandle(unsigned char key, int x, int y) {
        //Activates secondary function
        handleKey(key, x, y);
    }
    void updateHold(int value) {
    if (engine::current) update(value);
    }

    void engine::endProgram() {
        // program deleted stuff;
        delete engine::current->original;
    }
    void handleKey(char key, int x, int y) {
        //If users inputs q they end program.
    if (key == 27 || key == 'q' || key == 'Q') {
        engine::endProgram();
        exit(0);
    }
    }
    void engine::draw() {
        //Get's shapes.
    for (auto& x : engine::otherBalls) x.drawShape();
    }
    engine::~engine() { delete original; }
