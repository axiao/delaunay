/*
 * Linkrings are the main data structure for representing our triangulation.
 * each linkring literally contains the star of outgoing edges from a central
 * vertex.
 *
 * Linkrings of every vertex in the triangulation will capture all edges
 * (and thus all triangles), as well.
 *
 */

#ifndef LINKRING_H_
#define LINKRING_H_

#include "vertex.h"

// an individual link in the cyclic data structure
class Link 
{
    public:
        Link();
        Link(const vertex&);
        ~Link();
        vertex val;
        Link* prev;
        Link* next;
};

// class for link rings (see Blelloch et al)
class Linkring
{
    public:
        Linkring();
        ~Linkring();
        // returns the index of v in the ring, or -1 if it can't find it
        // locate's results may change only after an add_link() or del_link()
        int locate(const vertex&);
        // insert a Link for v before index
        void add_link(const vertex&, int);
        // delete the link at index
        void del_link(int);
        bool empty;
        int size;
    private:
        Link* star;
};

#endif
