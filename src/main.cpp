// currently, testing

#include <vector>
#include <functional>
#include <iostream>
#include <math.h>

#include "geometric.h"
#include "vertex.h"
#include "linkring.h"
#include "triangulation.h"
#include "divide_and_conquer.h"

using namespace std;
using namespace geometric;

int main() {

    // testing vertices
    int hole = -1;
    int ghost = 0;
    cout << "-1 is ghost? " << is_ghost(hole) << endl;
    cout << "-1 is hole? " << is_hole(hole) << endl;
    cout << "0 is ghost? " << is_ghost(ghost) << endl;
    cout << "0 is hole? " << is_hole(ghost) << endl;

    Point2 points[] = {Point2(0,0), Point2(1,1), Point2(2,2), Point2(3,3), Point2(4,4), Point2(5,5), Point2(6,6), Point2(7,7)};
    vector<Point2>* ps = new vector<Point2>(points, points + sizeof(points)/sizeof(points[0]));
    vertex_buffer* p = new vertex_buffer(ps);
    vertex v[] = {7, 6, 5, 4, 3, 2, 1};
    lexico_sort(v, 7, p);
    for (int i = 0; i < 7; ++i) {
        cout << v[i] << endl;
    }

/*
    // testing triangulations
    Triangulation* t = new Triangulation();

    // NOTE: the first vertex is necessarily the ghost vertex
    Point2 points[] = {Point2(0,0), Point2(1,1), Point2(2,2), Point2(3,3), Point2(4,4), Point2(5,5), Point2(6,6), Point2(7,7)};
    V_BUFFER v_to_p = V_BUFFER(points);

    vertex a[] = {1, 2, 3, 4, 6};
    vector<vertex> v (a, a+5);

    t->init(v.begin(), v.end(), v_to_p);

    delete t;
*/

    return 0;

}
