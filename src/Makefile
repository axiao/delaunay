# Makefile for delaunay triangulator source

DT = $(shell echo `pwd`/../delaunay)

CXXFLAGS = -c -g -Wall -std=c++0x

# List of C++ sources
SRCS = main.cpp triangulation.cpp linkring.cpp

# List of C++ object files
OBJS = $(SRCS:.cpp=.o)

.PHONY: default compile ../delaunay .cpp.o check clean

default: compile

compile: ../delaunay

../delaunay: $(OBJS)
	$(CXX) $(OBJS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

check:
	$(MAKE) -C .. check

clean:
	$(RM) *~ *.o
