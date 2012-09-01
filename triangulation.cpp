

#include "triangulation.h"
#include "geometric.h"
#include <math.h>
#include <iostream>
#include <vector>

using namespace std;

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
template <typename Iter, typename Comp>
void Triangulation<T>::init(Iter start, Iter end, Comp comp) {
    // comp is a spatial comparison function, eg atan2
    cout << "hi" << endl; 
    this->comparator = comp;
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
bool comp(const int& x, const int& y) { return true; }
int main() {
    Triangulation<int>* t = new Triangulation<int>();
    int a[] = {1, 2, 3, 4, 6};
    vector<int> v (a, a+5);
    t->init(v.begin(), v.end(), comp);
    delete t;

    return 1;
}
