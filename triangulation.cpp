#include "triangulation.h"

#include <iostream>


Triangulation::Triangulation() {
    linkrings = new std::map<vertex, Linkring*>();
}

Triangulation::~Triangulation() {
    typename std::map<vertex, Linkring*>::iterator start, end, it; 
    start = linkrings->begin();
    end = linkrings->end();
    for (it = start; it != end; ++it) {
        delete it->second; // delete the Linkring object
    }
    delete linkrings; // delete the map of linkrings
}

void Triangulation::init(Iter start, Iter end, V_BUFFER vbf) {
    std::cout << "initing ... ";
    v_buf = vbf;
    std::pair<vertex, Linkring*> lrpair;
    for (Iter it = start; it != end; ++it) {
        lrpair = std::pair<vertex, Linkring*>(*it, new Linkring()); 
        linkrings->insert(lrpair);
    }
    std::cout << "and done." << std::endl;
}

void Triangulation::add_triangle(const vertex& u, const vertex& v, const vertex& w) {
    // TODO
    // update u's linkring
    add_to_star(u, v, w);
    // update v's linkring
    add_to_star(v, w, u);
    // update w's linkring
    add_to_star(w, u, v);
}

void Triangulation::rm_triangle(const vertex& u, const vertex& v, const vertex& w) {
    // TODO
    // update u's linkring
    rm_from_star(u, v, w);
    // update v's linkring
    rm_from_star(v, w, u);
    // update w's linkring
    rm_from_star(w, u, v);
}

void Triangulation::add_to_star(const vertex& sv, const vertex& v1, const vertex& v2) { 
    // TODO not implemented, do we need this?
    // CASE: sv's star is empty (no edges)
    //if linkrings
    // CASE: sv's star has v1 but not v2
    // CASE: sv's star has v2 but not v1
    // CASE: sv's star has both v1 and v2, but they are separated by ghost/hole
    // CASE: sv's star has both v1 and v2, but they are separated by another v
    
}

void Triangulation::rm_from_star(const vertex& sv, const vertex& v1, const vertex& v2) {
    // TODO not implemented
}
