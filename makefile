CXX = g++
CXXFLAGS = -Wall -g
LDLIBS = -lGL -lGLU -lglut -lGLEW -lSDL2 -lSDL2_image -lfreetype
#Old make values for testing file
TARGET = simulacrum
# Rule to build and run specifically for your testing file
simul: testing/simulacrum.cpp src/ball.cpp src/wall.cpp src/object.h src/ball.h src/quadtree.cpp src/quadtree.h
	$(CXX) $(CXXFLAGS) testing/simulacrum.cpp src/ball.cpp src/quadtree.cpp  -o $(TARGET) $(LDLIBS)
	./$(TARGET)


# new
file = main
a:
	@echo "dsdss"
# My main file
main: testing/simulacrum.cpp src/ball.cpp main.cpp src/wall.cpp src/object.h src/ball.h src/quadtree.cpp src/quadtree.h src/engine.h src/engine.cpp
	$(CXX) $(CXXFLAGS) src/engine.cpp main.cpp src/ball.cpp src/quadtree.cpp  -o $(file) $(LDLIBS)
	./$(file)

