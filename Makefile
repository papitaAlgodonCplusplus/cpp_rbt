CXX = g++
CXXFLAGS = -std=c++11 -Iinclude

SRCS = src/Robot.cpp src/RobotAsalto.cpp src/RobotExplorador.cpp src/RobotDefensor.cpp src/RedBlackTree.cpp
OBJS = $(SRCS:.cpp=.o)

main: $(OBJS) main.o
	$(CXX) $(CXXFLAGS) -o main $(OBJS) main.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

clean:
	rm -f main $(OBJS) main.o
