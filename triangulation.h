/*
 * A triangulation class
 * uses the BBCK link ring data structure
 * and the interface given in J. Shewchuk's lecture notes
 */

#ifndef TRIANGULATION_H_
#define TRIANGULATION_H_

#include <vector>
#include <map>

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
        // class for link rings (see Blelloch et al)
        class Linkring
        {
            public:
                Linkring() {
                    this->empty = true;
                    this->star = new typename std::vector<T>();
                }
                ~Linkring() {
                    delete this->star;
                }
                T& operator[](const int idx) {
                    if (!this->empty) {
                        return (*star)[idx % star.size()];
                    }
                }
                // inserts an element before index idx
                void insert(const int idx, const T& x) {
                    star->insert(idx % star->size(), x);
                    this->empty = false;
                }
                // removes the element at index idx
                void remove(const int idx) {
                    if (!this->empty) {
                        star->erase(idx % star->size());
                        if (star->empty()) {
                            this->empty = true;
                        }
                    }
                }
            //private:
                bool empty;
                typename std::vector<T>* star;
        };
        // link rings around each vertex completely define all edges
        // and positional relationships in the triangulation
        void add_to_star(const T&, const T&, const T&);
        void rm_from_star(const T&, const T&, const T&);
        typename std::map<T, Linkring*>* linkrings;
        bool (*comparator)(const T&, const T&);
};

#endif
