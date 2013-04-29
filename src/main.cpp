// currently, testing

#include <getopt.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "geometric.h"
#include "vertex.h"
#include "linkring.h"
#include "triangulation.h"
#include "divide_and_conquer.h"

using namespace std;
using namespace geometric;


void print_help() {
    cout << "TODO HELP MESSAGE" << endl;
}


int main(int argc, char* argv[]) {

    size_t num_v, dim, num_attr, num_bd_m, vn, tri_count;
    double vx, vy;
    string line;
    ifstream nodefile;
    ofstream elefile;
    char *filename = NULL;
    char *outfile = "a.ele";

    int help = 0;
    int time = 0;
    string algorithm = "AC";
    int alg_choice;
    int c;
    while (true) {
        const struct option long_options[] =
            {
                /* flag options */
                {"help", no_argument, &help, 1},
                {"time", no_argument, &time, 1},
                /* argument options */
                {"algorithm", required_argument, NULL, 'a'},
                {"output", required_argument, NULL, 'o'},
                {0, 0, 0, 0}
            };
        int options_index = 0;

        c = getopt_long(argc, argv, "hta:", long_options, &options_index);
        if (c == -1) { 
            break; 
        }
        switch (c) {
        case 0:
            if (long_options[options_index].flag != 0) {
                break;
            }
            cout << "options " << long_options[options_index].name << endl;
            if (optarg) {
                cout << " with option " << optarg << endl;
            }
            break;
        case 'h':
            ++help;
            break;
        case 't':
            ++time;
            break;
        case 'a':
            cout << "option -a " << optarg << endl;
            algorithm = string(optarg);
            break;
        case 'o':
            cout << "option -o " << optarg << endl;
            outfile = optarg;
            break;
        default:
            cout << "huh?" << endl;
        }
    }
    // process non-options (arguments)
    cout << "time: " << time << endl;
    cout << "help: " << help << endl;
    if (!help and (optind + 1 != argc)) {
        cerr << "usage: delaunay [filename]" << endl;
        return 1;
    } else if (help) {
        print_help();
        return 0;
    } else {
        if (algorithm == "AC") {
            alg_choice = 0;
        } else if (algorithm == "VC") {
            alg_choice = 1;
        } else {
            cerr << "Error: unrecognized algorithm choice " << 
                algorithm  << endl;
            return 1;
        }
        filename = argv[optind];
        cout << "argument: " << filename << endl;

        cout << "===reading from file" << endl;
        nodefile.open(filename);
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
            vector<Point2>* vec_p = new vector<Point2>(points, points+num_v+1);
            vertex_buffer p(vec_p);

            exactinit();
            edge_pair le_re;

            switch (alg_choice) {
            case 0: // alternating cuts
                cout << "===triangulating ..." << endl;
                le_re = delaunay_dc2(vertices, num_v, true, p);
                break;
            case 1: // vertical cuts only
                cout << "===sorting vertices" << endl;
                lexico_sort(vertices, num_v, p);

                for (size_t i = 0; i < num_v; ++i) {
                    cout << vertices[i] << " ";
                    cout << p.val(vertices[i]) << " ";
                }
                cout << endl;
                cout << "===triangulating ..." << endl;
                le_re = delaunay_dc(vertices, num_v, p);
                break;
            default:
                cerr << "Error: unrecognized algorithm choice " << 
                    algorithm << endl;
            }


            cout << "num_v: " << num_v << endl;
            cout << "vertices: ";
            for (size_t i = 0; i < num_v; ++i) {
                cout << vertices[i] << " ";
            }
            cout << endl;
            cout << "left edge: (" << le_re[0].org() << "," << le_re[0].dst() << ")" << endl;
            cout << "right edge: (" << le_re[1].org() << "," << le_re[1].dst() << ")" << endl;


            string triangles = serialize_triangles(le_re[0]);
            tri_count = 0;
            string result;
            istringstream iss(triangles);
            for (string line; getline(iss, line);) {
                stringstream ss;
                ss << (++tri_count);
                result += ss.str() + " " + line + "\n";
            }
            stringstream ss;
            ss << tri_count;
            
            elefile.open(outfile);
            elefile << ss.str() << " 3 0\n";
            elefile << result;
            elefile.close();

        } else {
            cout << "unable to open input file" << endl;
            return 1;
        }

        return 0;
    }
}

