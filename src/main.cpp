// currently, testing

#include <getopt.h>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "geometric.h"
#include "vertex.h"
#include "linkring.h"
#include "triangulation.h"
#include "divide_and_conquer.h"

using namespace std;
using namespace geometric;


void print_usage() {
    cout << "usage: delaunay " << 
        "[-h | --help] [-t | --time] [-a | --algorithm <alg>] " << 
        "[-o <output file>] <file.node>" << endl;
}

void print_help() {
    cout << "A delaunay triangulation program." << endl;
    print_usage();
    cout << "command line options:" << endl;
    cout << "\t" << "-h, --help" << "\t" <<
        "display this message" << endl;
    cout << "\t" << "-o, --output" << "\t" <<
        "output file path " << "\n\t\t\t" <<
        "default: for argument *.node, makes *.ele" << endl;
    cout << "\t" << "-a, --algorithm" << "\t" <<
        "triangulation algorithm to use: " << "\n\t\t\t\t" <<
        "VC (divide/conquer vertical cuts) " << "\n\t\t\t\t" <<
        "AC (divide/conquer alternating cuts) " << "\n\t\t\t" <<
        "default: AC" << endl;
    cout << "\t" << "-t, --time" << "\t" <<
        "output triangulation running time (in seconds)" << endl;
}

string getFileExt(const string& s) {
    size_t i = s.rfind('.', s.length());
    if (i != string::npos) {
        return (s.substr(i + 1, s.length() - i));
    }
    return ("");
}

string getFileName(const string& s) {
    size_t i = s.rfind('.', s.length());
    if (i != string::npos) {
        return (s.substr(0, i));
    }
    return ("");
}


int main(int argc, char* argv[]) {

    size_t num_v, dim, num_attr, num_bd_m, vn, tri_count;
    double vx, vy;
    string line;
    ifstream nodefile;
    ofstream elefile;
    char *filename = NULL;
    char *outfile = NULL;
    clock_t time_start = 0;

    int help = 0;
    int timing = 0;
    string algorithm = "AC";
    int alg_choice;
    int c;
    while (true) {
        const struct option long_options[] =
            {
                /* flag options */
                {"help", no_argument, &help, 1},
                {"time", no_argument, &timing, 1},
                /* argument options */
                {"algorithm", required_argument, NULL, 'a'},
                {"output", required_argument, NULL, 'o'},
                {0, 0, 0, 0}
            };
        int options_index = 0;

        c = getopt_long(argc, argv, "h?ta:o:", long_options, &options_index);
        if (c == -1) { 
            break; 
        }
        switch (c) {
        case 'h':
            ++help;
            break;
        case 't':
            ++timing;
            break;
        case 'a':
            algorithm = string(optarg);
            break;
        case 'o':
            outfile = optarg;
            break;
        case '?':
            break;
        default:
            break;
        }
    }
    // process non-options (arguments)
    if (!help and (optind + 1 != argc)) {
        print_usage();
        return 1;
    } else if (help) {
        print_help();
        return 0;
    } else {
        if (algorithm == "AC") {
            cout << "Algorithm: alternating cuts" << endl;
            alg_choice = 0;
        } else if (algorithm == "VC") {
            cout << "Algorithm: vertical cuts only" << endl;
            alg_choice = 1;
        } else {
            cerr << "Error: unrecognized algorithm choice '" << 
                algorithm << "'" << endl;
            return 1;
        }
        filename = argv[optind];

        // verify file extension
        string extension = getFileExt(filename);
        string no_extension = getFileName(filename);
        if (extension != "node" or no_extension == "") {
            cerr << "Error: input must have file extension .node" << endl;
            return 1;
        }

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
                        cerr << "Error: .node dimension must be 2." << endl;
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


            if (timing) {
                time_start = clock();
            }

            switch (alg_choice) {
            case 0: // alternating cuts
                // alternating cuts starts on vertical cuts
                le_re = delaunay_dc2(vertices, num_v, true, p);
                break;
            case 1: // vertical cuts only
                // sort vertices first
                lexico_sort(vertices, num_v, p);
                le_re = delaunay_dc(vertices, num_v, p);
                break;
            default:
                cerr << "Error: unrecognized algorithm choice " << 
                    algorithm << endl;
            }

            if (timing) {
                cout << "Triangulated in: " << 
                    (float)(clock() - time_start)/CLOCKS_PER_SEC << 
                    " seconds" << endl;
            }

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
            
            if (outfile == NULL) {
                // compute the correct output filename (.node:.ele)
                no_extension = no_extension + ".ele";
                cout << "Output to: " << no_extension << endl;
                elefile.open(no_extension);
            } else {
                // otherwise, use the provided target
                cout << "Output to: " << outfile << endl;
                elefile.open(outfile);
            }
            ss << tri_count;
            elefile << ss.str() << " 3 0\n";
            elefile << result;
            elefile.close();

        } else {
            cerr << "Error: unable to open input file." << endl;
            return 1;
        }

        return 0;
    }
}

