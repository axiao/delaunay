/*
 * A triangulation class
 * uses the BBCK link ring data structure
 * and the interface given in J. Shewchuk's lecture notes
 */

#ifndef TRIANGULATION_H_
#define TRIANGULATION_H_

#include <vector>
#include <map>


template <typename T>
class Link // an individual link in the cyclic data structure
{
    public:
        Link();
        Link(const T&, bool, bool);
        ~Link();
    //private:
        T val;
        bool is_ghost;
        bool is_null;
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
        template <typename Iter>
        void init(Iter, Iter);
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
        void add_to_star(const T&, const T&, const T&);
        void rm_from_star(const T&, const T&, const T&);
        typename std::map<T, Linkring<T>*>* linkrings;
};


#endif
