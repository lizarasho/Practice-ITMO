CXX = g++
CXXFLAGS = -O3 -std=c++17

.PHONY: all main clean

all: main

main: fsm.o maths_course.o main.o
	$(CXX) $(CXXFLAGS) fsm.o maths_course.o main.o -o Maths-course

fsm.o maths_course.o main.o:
	$(CXX) $(CXXFLAGS) -c fsm.cpp
	$(CXX) $(CXXFLAGS) -c maths_course.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

clean:
	rm -f *.o Maths-course	