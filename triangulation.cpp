#include "triangulation.h"
#include "geometric.h"
#include <math.h>
#include <iostream>
#include <vector>

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
void Triangulation<T>::init(Iter start, Iter end, 
                            double (*ccw)(const T&, const T&)) {
    cout << "hi" << endl; 
    orientation = ccw;
    pair<T, Linkring<T>*> lrpair;
    for (Iter it = start; it != end; ++it) {
        lrpair = pair<T, Linkring<T>*>(*it, new Linkring<T>()); 
        this->linkrings->insert(lrpair);
    }
}
template <typename T>
void Triangulation<T>::add_triangle(const T& u, const T& v, const T& w) {
    // update u's linkring
    add_to_star(u, v, w);
    // update v's linkring
    add_to_star(v, w, u);
    // update w's linkring
    add_to_star(w, u, v);
}
template <typename T>
void Triangulation<T>::rm_triangle(const T& u, const T& v, const T& w) {
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
    // TODO not implemented
    // CASE: add onto hull
    T temp[] = {v1, v2};
    sort(temp, temp+1, orientation); // now temp[0], temp[1] are in ccw order
    // TODO
    // CASE: add onto hull, opposite edge is against another triangle
    // CASE: add onto hull, adjacent edge is on another triangle
    
}
template <typename T> 
void Triangulation<T>::rm_from_star(const T& sv, const T& v1, const T& v2) {
    // TODO not implemented
}




// testing only
#include "geometric.h"
double temp(const int& i, const int& j) { return i*j;}
int main() {
    Triangulation<int>* t = new Triangulation<int>();
    int a[] = {1, 2, 3, 4, 6};
    vector<int> v (a, a+5);
    t->init(v.begin(), v.end(), &temp);
    delete t;

    int b = -1;
    int c = 0;
    std::cout << "-1 is ghost? " << VERTEX<int>::is_ghost(b) << std::endl;
    std::cout << "-1 is hole? " << VERTEX<int>::is_hole(b) << std::endl;
    std::cout << "0 is ghost? " << VERTEX<int>::is_ghost(c) << std::endl;
    std::cout << "0 is hole? " << VERTEX<int>::is_hole(c) << std::endl;

    return 1;
}
