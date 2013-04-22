/**
 * Main triangulation class.
 * Follows the pattern described by Blelloch et al. using link rings to
 * represent relationships between triangulation vertices.
 *
 */

#ifndef TRIANGULATION_H_
#define TRIANGULATION_H_

#include <iterator>
#include <map>
#include "geometric.h"
#include "vertex.h"
#include "linkring.h"

typedef std::vector<vertex>::iterator Iter; // change me as needed

class Triangulation
{
    public:
        Triangulation();
        ~Triangulation();
        // initialization method, takes in iterator range for input vertices
        // and a V_BUFFER which maps those vertices to 2d points
        void init(Iter, Iter, V_BUFFER);
        // adds positively oriented triangle uvw
        void add_triangle(const vertex&, const vertex&, const vertex&);
        // removes positively oriented triangle uvw
        void rm_triangle(const vertex&, const vertex&, const vertex&);
        // returns vertex w such that uvw is positively oriented triangle
        vertex adjacent(const vertex&, const vertex&);
        // omitted: adjacent_one()
    private:
        // add edge bc into link of a if legal
        void add_to_star(const vertex& a, const vertex& b, const vertex& c);
        // remove edge bc from link of a if possible
        void rm_from_star(const vertex& a, const vertex& b, const vertex& c);
        typename std::map<vertex, Linkring*>* linkrings;
        V_BUFFER v_buf;
};

#endif
