/**
 * Guibas and Stolfi's quad edge data structure for Voronoi diagrams and 
 * Delaunay Triangulations.
 */

#ifndef QUADEDGE_H_
#define QUADEDGE_H_

#include <string>

#include "vertex.h"

class Quadedge;
struct Edge_Record;
struct Q_Record;

// describes a Q_Record in a Quadedge... q->e[r]
typedef struct Edge_Record
{
    Edge_Record();
    Edge_Record(Quadedge*, int);
    Q_Record& get_qr();

    Edge_Record rot();
    Edge_Record sym();
    Edge_Record rot_inv();
    Edge_Record onext();

    Edge_Record oprev();
    Edge_Record lnext();
    Edge_Record lprev();
    Edge_Record rnext();
    Edge_Record rprev();

    vertex org();
    vertex dst();

    void set_org(vertex);
    void set_dst(vertex);

    Quadedge* q;
    int r;
} edge;

struct Q_Record 
{
    Q_Record();
    Q_Record next();
    void set_next(Edge_Record);

    Edge_Record n_er;
    vertex v;
    void* data;

};

class Quadedge 
{
    public:
        Quadedge();
        ~Quadedge();
        Edge_Record rot(int);
        Edge_Record sym(int);
        Edge_Record rot_inv(int);
        Edge_Record onext(int);

        Q_Record e[4]; // e[r] points to (e_0 Rot^r)
};

// basic topological operators
Edge_Record make_edge();
void splice(Edge_Record, Edge_Record);

// derived topological operators
edge connect(edge, edge);
void delete_edge(edge);
void swap(edge);
edge make_edge(vertex, vertex);

// printing the triangles
std::string serialize_triangles(edge); //?

#endif
