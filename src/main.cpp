// currently, testing

#include <vector>
#include <functional>
#include <iostream>
#include <math.h>



#include "geometric.h"
#include "vertex.h"
#include "linkring.h"
#include "triangulation.h"

using namespace std;


int main() {

    // testing vertices
    int hole = -1;
    int ghost = 0;
    cout << "-1 is ghost? " << is_ghost(hole) << endl;
    cout << "-1 is hole? " << is_hole(hole) << endl;
    cout << "0 is ghost? " << is_ghost(ghost) << endl;
    cout << "0 is hole? " << is_hole(ghost) << endl;


    // testing triangulations
    Triangulation* t = new Triangulation();

    // NOTE: the first vertex is necessarily the ghost vertex
    Point2 points[] = {Point2(0,0), Point2(1,1), Point2(2,2), Point2(3,3), Point2(4,4), Point2(5,5), Point2(6,6), Point2(7,7)};
    V_BUFFER v_to_p = V_BUFFER(points);

    vertex a[] = {1, 2, 3, 4, 6};
    vector<vertex> v (a, a+5);

    t->init(v.begin(), v.end(), v_to_p);
    delete t;


    return 0;

}
