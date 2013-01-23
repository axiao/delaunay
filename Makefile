# Makefile for delaunay triangulator

DT = $(shell echo `pwd`/delaunay)

CXXFLAGS = -c -g -Wall -std=c++0x

# List of C++ sources
SRCS = main.cpp triangulation.cpp linkring.cpp

# List of C++ object files
OBJS = $(SRCS:.cpp=.o)

.PHONY: default delaunay .cpp.o check clean

default: delaunay

delaunay: $(OBJS)
	$(CXX) $(OBJS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

check:
	echo 'make check has not been written yet'

clean:
	$(RM) *~ *.o delaunay
