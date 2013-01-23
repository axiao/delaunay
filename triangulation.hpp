/*
 * A triangulation class
 * uses the BBCK link ring data structure
 * and the interface given in J. Shewchuk's lecture notes
 */

#ifndef TRIANGULATION_H_
#define TRIANGULATION_H_

#include <math.h>
#include <iostream>
#include <vector>
#include <map>
#include <functional>



// representing hole and ghost vertices
template <typename T>
struct VERTEX
{
    // returns true iff v is the representation of the ghost vertex
    static bool is_ghost(const T& v);
    // returns true iff v is the representation of the hole vertex
    static bool is_hole(const T& v);
};

template <typename T>
class Link // an individual link in the cyclic data structure
{
    public:
        Link();
        Link(const T&);
        ~Link();
        // fields
        T val;
        Link<T>* prev;
        Link<T>* next;
};

// TODO REDO with Links to include NULL for holes, GHOST for hull
// class for link rings (see Blelloch et al)
template <typename T>
class Linkring
{
    public:
        Linkring();
        ~Linkring();
        // returns the index of v, or -1 if it can't find it
        int locate(const T&);
        // insert a Link for v before index
        void add_link(const T&, int);
        // delete the link at index
        void del_link(int);
        // fields
        bool empty;
        int size;
        Link<T>* star;
};

// main triangulation class
template <typename T>
class Triangulation
{
    public:
        Triangulation();
        ~Triangulation();
        // initialization method, takes in iterator range for inputs
        // and an evaluation function to compare points
        template <typename Iter>
        void init(Iter, Iter, std::function<double (const T&, const T&)> (const T&));//double (*)(const T&, const T&));
        // adds positively oriented triangle uvw
        void add_triangle(const T&, const T&, const T&);
        // removes positively oriented triangle uvw
        void rm_triangle(const T&, const T&, const T&);
        // returns vertex w such that uvw is positively oriented triangle
        T adjacent(const T&, const T&);
        // omitted: adjacent_one()
    private:
        // link rings around each vertex completely define all edges
        // and positional relationships in the triangulation
        // add edge bc into link of a if legal
        void add_to_star(const T& a, const T& b, const T& c);
        // remove edge bc from link of a if possible
        void rm_from_star(const T& a, const T& b, const T& c);
        typename std::map<T, Linkring<T>*>* linkrings;
        // gives an ordering of vertex v about the vertex center
        //double (*orientation)(const T& center, const T& v);
        std::function<double (const T&, const T&)> (*orient) (const T&);
};


/*
 * ============================================================================
 * definitions for template classes
 * ============================================================================
 */

#include "geometric.h"

using namespace std;


// Vertex attributes
// T = int
template <>
struct VERTEX<int> {
    static bool is_ghost(const int& v) { return v == 0; }
    static bool is_hole(const int& v) { return v == -1; }
};

// Links
template <typename T>
Link<T>::Link() {
    val = 0;
    prev = 0;
    next = 0;
}
template <typename T>
Link<T>::Link(const T& value) {
    val = value;
    prev = 0;
    next = 0;
}
template <typename T>
Link<T>::~Link() { }


// Linkrings
template <typename T>
Linkring<T>::Linkring() {
    empty = true;
    size = 0;
}
template <typename T>
Linkring<T>::~Linkring() {
    if (!empty) {
        Link<T>* nxt;
        Link<T>* it = star;
        do {
            nxt = it->next;
            delete it;
            it = nxt;
        } while (it != star);
    }
}
template <typename T>
int Linkring<T>::locate(const T& v) {
    // returns the index of v, or -1 if it can't find it
    if (empty) {
        return -1;
    }
    Link<T>* target = star;
    int ret = 0;
    do {
        if (target->val == v) {
            return ret;
        }
        target = target->next; 
        ++ret;
    } while (target != star);
    return -1;
}
template <typename T>
void Linkring<T>::add_link(const T& v, int index) {
    // insert a Link for v before index
    if (empty) {
        star = new Link<T>(v);
        empty = false;
    } else {
        Link<T>* target = star;
        for (int i = 0; i < index; ++i) {
            target = target->next;
        } 
        target->prev->next = new Link<T>(v);
        target->prev = target->prev->next;
    }
    ++size;
}
template <typename T>
void Linkring<T>::del_link(int index) {
    // delete the link at index
    Link<T>* target = star;
    for (int i = 0; i < index; ++i) {
        target = target->next;
    }
    if ((target->next != 0) && (target->prev != 0)) {
        target->next->prev = target->prev;
        target->prev->next = target->next;
    }
    delete target;
    size--;
    if (size == 0) {
        empty = true;
    }
}


// Triangulation
template <typename T> 
Triangulation<T>::Triangulation() {
    this->linkrings = new map<T, Linkring<T>*>();
}
template <typename T> 
Triangulation<T>::~Triangulation() {
    typename map<T, Linkring<T>*>::iterator start, end, it; 
    start = this->linkrings->begin();
    end = this->linkrings->end();
    for (it = start; it != end; ++it) {
        delete it->second; // delete the Linkring object
    }
    delete this->linkrings; // delete the map of linkrings
}
template <typename T> 
template <typename Iter>
void Triangulation<T>::init(Iter start, Iter end, std::function<double (const T&, const T&)> (*o) (const T&)) {
    cout << "hi" << endl; 
    orient = o;
    pair<T, Linkring<T>*> lrpair;
    for (Iter it = start; it != end; ++it) {
        lrpair = pair<T, Linkring<T>*>(*it, new Linkring<T>()); 
        this->linkrings->insert(lrpair);
    }
}
template <typename T>
void Triangulation<T>::add_triangle(const T& u, const T& v, const T& w) {
    // TODO
    // update u's linkring
    add_to_star(u, v, w);
    // update v's linkring
    add_to_star(v, w, u);
    // update w's linkring
    add_to_star(w, u, v);
}
template <typename T>
void Triangulation<T>::rm_triangle(const T& u, const T& v, const T& w) {
    // TODO
    // update u's linkring
    rm_from_star(u, v, w);
    // update v's linkring
    rm_from_star(v, w, u);
    // update w's linkring
    rm_from_star(w, u, v);
}
// private methods
template <typename T> 
void Triangulation<T>::add_to_star(const T& sv, const T& v1, const T& v2) { 
    // TODO not implemented, do we need this?
    //T temp[] = {v1, v2};
    //sort(temp, temp+1, orient(sv)); // now temp[0], temp[1] are in ccw order
    // TODO
    // CASE: sv's star is empty (no edges)
    //if this->linkrings
    // CASE: sv's star has v1 but not v2
    // CASE: sv's star has v2 but not v1
    // CASE: sv's star has both v1 and v2, but they are separated by ghost/hole
    // CASE: sv's star has both v1 and v2, but they are separated by another v
    
}
template <typename T> 
void Triangulation<T>::rm_from_star(const T& sv, const T& v1, const T& v2) {
    // TODO not implemented
}

/*
 * ----------------------------------------------------------------------------
 * end template class definitions
 * ============================================================================
 */


#endif
