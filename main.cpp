// currently, testing

#include <vector>
#include <functional>
#include <iostream>
#include <math.h>

#include "geometric.h"
#include "triangulation.hpp"

std::function<double (const int&, const int&)> temp(const int& t) { 
    return [](const int& a, const int& b){return a + b;};
}

int main() {

    Triangulation<int>* t = new Triangulation<int>();
    int a[] = {1, 2, 3, 4, 6};
    std::vector<int> v (a, a+5);
    t->init(v.begin(), v.end(), &temp);
    delete t;

    int b = -1;
    int c = 0;
    std::cout << "-1 is ghost? " << VERTEX<int>::is_ghost(b) << std::endl;
    std::cout << "-1 is hole? " << VERTEX<int>::is_hole(b) << std::endl;
    std::cout << "0 is ghost? " << VERTEX<int>::is_ghost(c) << std::endl;
    std::cout << "0 is hole? " << VERTEX<int>::is_hole(c) << std::endl;

    return 0;
}
