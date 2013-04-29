# Makefile for delaunay triangulator source

DT = $(shell echo `pwd`/../delaunay)

CXXFLAGS = -c -g -Wall -std=c++0x -O3
CCFLAGS = -c -O3

# List of C++ sources
SRCS = main.cpp \
	quadedge.cpp \
	divide_and_conquer.cpp

# List of C sources
CSRCS = predicates.c

# List of C++ object files
OBJS = $(SRCS:.cpp=.o)
COBJS = $(CSRCS:.c=.o)

.PHONY: default compile .cpp.o check clean

default: compile

compile: ../delaunay

../delaunay: $(COBJS) $(OBJS) 
	$(CXX) $(OBJS) $(COBJS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

.c.o:
	$(CC) $(CCFLAGS) $< -o $@

check:
	$(MAKE) -C .. check

clean:
	$(RM) *~ *.o
