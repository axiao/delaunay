// currently, testing

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

#include "geometric.h"
#include "vertex.h"
#include "linkring.h"
#include "triangulation.h"
#include "divide_and_conquer.h"

using namespace std;
using namespace geometric;

int main() {

    size_t num_v, dim, num_attr, num_bd_m, vn;
    double vx, vy;

    // TODO make an array as well

    // read vertices in from input
    string line;
    ifstream nodefile;

    nodefile.open("test.node");
    if (nodefile.is_open()) {
        while(getline(nodefile >> ws, line)) {
            if (line != "" and line[0] != '#') {
                istringstream iss(line);
                // parse the first line
                iss >> num_v; 
                iss >> dim;
                iss >> num_attr;
                iss >> num_bd_m;
                if (dim != 2) {
                    cout << "dimension must be 2" << endl;
                    return 1;
                }
                // TODO more stuff for boundary markers, attributes
                break;
                
            }
        }
        Point2 points[num_v+1];
        while(getline(nodefile >> ws, line)) {
            if (line != "" and line[0] != '#') {
                istringstream iss(line);
                // parse other .node lines
                // <vertex no.> <x> <y> [attrs] [bdy marker]
                iss >> vn;
                iss >> vx;
                iss >> vy;
                // TODO assert vn was not already used?
                points[vn] = Point2(vx, vy);
            }
        }

        // make the vertex[] and vector<Point2>
        vertex vertices[num_v];
        for (size_t i = 0; i < num_v; ++i) {
            vertices[i] = i + 1;
        }
        vector<Point2>* vec_p = new vector<Point2>(points+1, points+num_v+1);
        vertex_buffer p(vec_p);

        // FIXME
        edge_pair le_re;
        lexico_sort(vertices, num_v, p);
        cout << "sorted verts: ";
        for (size_t i = 0; i < num_v; ++i) {
            cout << vertices[i] << " ";
        }
        cout << endl;
        le_re = delaunay_dc(vertices, num_v, p);

        cout << "num_v: " << num_v << endl;
        cout << "vertices: ";
        for (size_t i = 0; i < num_v; ++i) {
            cout << vertices[i] << " ";
        }
        cout << endl;
        cout << "left edge: (" << le_re[0].org() << "," << le_re[0].dst() << ")" << endl;
        cout << "right edge: (" << le_re[1].org() << "," << le_re[1].dst() << ")" << endl;
        cout << "... they are the same?! something is totally broken." << endl;


        //cout << serialize_triangles(le_re[1]) << endl;


    } else {
        cout << "unable to open file" << endl;
        return 1;
    }

    //delaunay_dc(

    return 0;

}
