# Makefile for delaunay triangulator

DT = $(shell echo `pwd`/delaunay)

CXXFLAGS = -c -g -Wall -std=c++0x

all: delaunay

#delaunay: main.o triangulation.o
#	$(CXX) main.o triangulation.o -o delaunay
#
#main.o: main.cpp
#	$(CXX) $(CXXFLAGS) main.cpp
#
#triangulation.o: triangulation.cpp
#	$(CXX) $(CXXFLAGS) triangulation.cpp
#
#clean:
#	$(RM) *~ *.o delaunay

delaunay: main.o
	$(CXX) main.o -o delaunay

main.o: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp

#triangulation.o: triangulation.hpp
#	$(CXX) $(CXXFLAGS) triangulation.hpp

clean:
	$(RM) *~ *.o delaunay
