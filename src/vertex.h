/**
 * Vertices are represented as integers.
 *
 * General vertices (which correspond to real planar coordinates) have
 * strictly positive value.
 *
 * The GHOST vertex (which helps define the hull of a triangulation) is 0.
 * The HOLE vertex (which helps define holes in a triangulation) is -1.
 *
 * A V_BUFFER maps vertices to planar coordinates.
 *
 */

#ifndef VERTEX_H_
#define VERTEX_H_

#include <vector>
#include "geometric.h"

typedef int vertex;

// TODO? make this into a proper class?
typedef std::vector<Point2>* V_BUFFER;

// returns true iff v is the representation of the ghost vertex
inline bool is_ghost(const vertex& v) { 
    return v == 0; 
}

// returns true iff v is the representation of the hole vertex
inline bool is_hole(const vertex& v) { 
    return v == -1; 
}

#endif
