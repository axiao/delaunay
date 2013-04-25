/**
 * Divide and conquer algorithm for computing a Delaunay triangulation.
 * Uses the pseudocode from Guibas and Stolfi, which in turn is an algorithm
 * of Lee and Schachter.
 *
 * The method of alternating coordinate cuts is motivated by a paper by Dwyer.
 */

#ifndef DIVIDE_AND_CONQUER_H_
#define DIVIDE_AND_CONQUER_H_

#include "vertex.h"
#include "quadedge.h"

struct edge_pair {
public:
    edge_pair();
    edge_pair(edge, edge);
    edge operator[](size_t);
private:
    edge e0;
    edge e1;
};

// returns two edges (le, re) in an array, which adjoin the leftmost/rightmost 
// point of the convex hull respectively
edge_pair delaunay_dc(vertex[], size_t, vertex_buffer*);
// uses alternating coordinate cuts and partitioning instead of initial sort
edge_pair delaunay_dc2(vertex[], size_t, vertex_buffer*);

// sorts the vertex set by x-coordinate, breaking ties by y-coordinate
void lexico_sort(vertex[], size_t, vertex_buffer*);
size_t partition(vertex[], size_t, size_t, size_t, vertex_buffer*);
void quicksort(vertex[], size_t, size_t, vertex_buffer*);

// partitions the vertex set about the medial element on the x or y axis
// returns the index of the index of the medial element (ie quickselect)
int lexico_partition(vertex[], size_t, bool, vertex_buffer*);

#endif
