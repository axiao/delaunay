/**
 * Vertices are represented as integers.
 *
 * General vertices (which correspond to real planar coordinates) have
 * strictly positive value.
 *
 * The GHOST vertex (which helps define the hull of a triangulation) is 0.
 * The HOLE vertex (which helps define holes in a triangulation) is -1.
 *
 * A vertex_buffer maps vertices to planar coordinates.
 *
 */

#ifndef VERTEX_H_
#define VERTEX_H_

#include <vector>
#include "geometric.h"

typedef int vertex;

// essentially a vertex->Point2 translation class
class vertex_buffer {
public:
    vertex_buffer(std::vector<geometric::Point2>* buf): 
        vbf(buf) { }
    geometric::Point2& val(vertex i) {
        return (*vbf)[i];
    }
    double distance(vertex i, vertex j) {
        return geometric::distance(val(i), val(j));    
    }
    double orient2d(vertex i, vertex j, vertex k) {
        return geometric::orient2d(val(i), val(j), val(k));
    }
    double incircle(vertex i, vertex j, vertex k, vertex l) {
        return geometric::incircle(val(i), val(j), val(k), val(l));
    }
    bool leftof(vertex i, vertex j, vertex k) {
        return geometric::leftof(val(i), val(j), val(k));
    }
    
    bool rightof(vertex i, vertex j, vertex k) {
        return geometric::rightof(val(i), val(j), val(k));
    }
private:
    std::vector<geometric::Point2>* vbf;
};

// returns true iff v is the representation of the ghost vertex
inline bool is_ghost(const vertex& v) { 
    return v == 0; 
}

// returns true iff v is the representation of the hole vertex
inline bool is_hole(const vertex& v) { 
    return v == -1; 
}

#endif
