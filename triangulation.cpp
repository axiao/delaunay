

#include "triangulation.h"
#include <iostream>
#include <vector>

using namespace std;

template <typename T> 
Triangulation<T>::Triangulation() {
    this->linkrings = new map<T, Linkring*>();
}

template <typename T> 
Triangulation<T>::~Triangulation() {
    typename map<T, Linkring*>::iterator start, end, it; 
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
    for (Iter it = start; it != end; ++it) {
        pair<T, Linkring*> lrpair = pair<T, Linkring*>(*it, new Linkring()); 
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
    // TODO
}


// private methods

// helper function to add new vertices to a link ring
template <typename T> 
void Triangulation<T>::add_to_star(const T& sv, const T& v1, const T& v2) {
    vector<T>* star;
    typename vector<T>::iterator it, pos1, pos2;
    bool contain1, contain2;
    star = this->linkrings[sv]->star;
    pos1 = find(star.begin(), star.end(), v1);
    pos2 = find(star.begin(), star.end(), v2);
    contain1 = pos1 != star.end();
    contain2 = pos2 != star.end();
    if (!(contain1 && contain2)) {
        // neither of the other vertices are  connected! 
        // we'll attach both and use the 
        // spatial sorting function that was provided in init().
        star->push_back(v1); 
        star->push_back(v2); 
        sort(star.begin(), star.end(), this->comparator);
    } else if (contain1) {
        star->insert(pos1+1, v2);
    } else if (contain2) {
        star->insert(pos2, v1);
    }
}

template <typename T> 
void Triangulation<T>::rm_from_star(const T& sv, const T& v1, const T& v2) {
    // TODO
}

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
