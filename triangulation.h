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
        Link() {
            val = 0;
            linkprev = false; 
            linknext = false; 
            prev = 0;
            next = 0;
        }
        Link(const T& value) {
            val = value;
            linkprev = false; 
            linknext = false; 
            prev = 0;
            next = 0;
        }
        ~Link() { }
        void link_prev() {
            if (prev != 0) {
                linkprev = true;
            }
        }
        void link_next() {
            if (next != 0) {
                linknext = true;
            }
        }
        void unlink_prev() {
            linkprev = false;
        }
        void unlink_next() {
            linknext = false;
        }
    //private:
        T val;
        bool linkprev; 
        bool linknext;
        Link<T>* prev;
        Link<T>* next;
};

// TODO REDO with Links to include NULL for holes, GHOST for hull
// class for link rings (see Blelloch et al)
template <typename T>
class Linkring
{
    public:
        Linkring() {
            empty = true;
            size = 0;
        }
        ~Linkring() {
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
        int locate(const T& v) {
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
        void add_link(const T& v, int index) {
            // insert a Link for v before index
            if (empty) {
                star = new Link<T>(v);
                empty = false;
            } else {
                Link<T>* target = star;
                for (int i = 0; i < index; ++i) {
                    target = target->next;
                } 
                // target and target->prev should not be linked
                target->prev->next = new Link<T>(v);
                target->prev = target->prev->next;
            }
            ++size;
        }
        void del_link(int index) {
            // delete the link at index
            // for most purposes, only del_link() unlinked nodes
            Link<T>* target = star;
            for (int i = 0; i < index; ++i) {
                target = target->next;
            }
            if ((target->next != 0) && (target->prev != 0)) {
                target->next->unlink_prev(); 
                target->next->prev = target->prev;
                target->prev->unlink_next(); 
                target->prev->next = target->next;
            }
            delete target;
            size--;
            if (size == 0) {
                empty = true;
            }
        }
        void connect_next(int index) {
            // links the Link at index to its next
            Link<T>* target = star;
            for (int i = 0; i < index; ++i) {
                target = target->next;
            }
            target->link_next();
            target->next->link_prev();
        }
        bool linked(const T& v1, const T& v2) {
            // returns true if there is a positive triangle this, v1, v2
            // ie the Links for v1 and v2 are linked
            Link<T>* target = star;
            do {
                if (target->val == v1) {
                    if (target->next->val == v2) {
                        if (target->linknext && target->next->linkprev) {
                            return true;
                        }
                    } 
                    return false;
                } else {
                    target = target->next;
                }
            } while (target != star);
            return false;
        }
        bool empty;
        int size;
        Link<T>* star;
};

#endif


// main triangulation class
template <typename T>
class Triangulation
{
    public:
        Triangulation();
        ~Triangulation();
        // initialization method, takes in iterator range for inputs
        template <typename Iter, typename Comp>
        void init(Iter, Iter, Comp);
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
        bool (*comparator)(const T&, const T&);
};


