

#include "triangulation.h"
#include "geometric.h"
#include <math.h>
#include <iostream>
#include <vector>

using namespace std;


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
void Triangulation<T>::init(Iter start, Iter end) {
    cout << "hi" << endl; 
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
    // not implemented
}
template <typename T> 
void Triangulation<T>::rm_from_star(const T& sv, const T& v1, const T& v2) {
    // not implemented
}
/*
// helper function to add new vertices to a link ring
template<>
void Triangulation<Point2>::add_to_star(const Point2& sv, const Point2& v1, const Point2& v2) {
    // TODO: reject if triangle already exists on any of the edges
    vector<Point2>* star;
    //map<Point2, Linkring<Point2>*> lr;
    map<Point2, Linkring<Point2>*>* lr;
    typename vector<Point2>::iterator it, pos1, pos2;
    bool contain1, contain2;
    lr = this->linkrings;
    star = new vector<Point2>();
    //star = this->linkrings[sv]->star;
    pos1 = find(star->begin(), star->end(), v1);
    pos2 = find(star->begin(), star->end(), v2);
    contain1 = pos1 != star->end();
    contain2 = pos2 != star->end();
    if (!(contain1 && contain2)) {
        
    } else if (contain1) {

    } else if (contain2) {

    }
}

template<>
void Triangulation<Point2>::rm_from_star(const Point2& sv, const Point2& v1, const Point2& v2) {
    // TODO: reject if triangle doesn't exist
}
*/
// testing only
#include "geometric.h"
int main() {
    Triangulation<int>* t = new Triangulation<int>();
    int a[] = {1, 2, 3, 4, 6};
    vector<int> v (a, a+5);
    t->init(v.begin(), v.end());
    delete t;

    return 1;
}
