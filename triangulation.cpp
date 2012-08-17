

#include "triangulation.h"
#include <iostream>
#include <vector>

template <typename T> 
Triangulation<T>::Triangulation() {
    this->linkrings = new typename std::map<T, Linkring>();
}

template <typename T> 
Triangulation<T>::~Triangulation() {
    delete this->linkrings;
}

template <typename T> 
template <typename Iter>
void Triangulation<T>::init(Iter start, Iter end) {
    // TODO take in a collection of T, or an iterator??
    std::cout << "hi" << std::endl; 
    for (Iter it = start; it != end; ++it) {
        this->linkrings->insert(*it);
    }
}

template <typename T>
void add_triangle(const T& u, const T& v, const T& w) {
    // TODO
}

template <typename T>
void rm_triangle(const T& u, const T& v, const T& w) {
    // TODO
}

// testing only
#include "geometric.h"
int main() {
    Triangulation<int>* t = new Triangulation<int>();
    int a[] = {1, 2, 3, 4, 6};
    std::vector<int> v (a, a+5);
    t->init(v.begin(), v.end());
    
    return 1;
}
