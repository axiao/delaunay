#include <iostream>
#include <sstream>
#include <stack>

#include "quadedge.h"

// Edge_Record
Edge_Record::Edge_Record() {

}

Edge_Record::Edge_Record(Quadedge* qe, int rot): 
    q(qe), r(rot) { };

Q_Record& Edge_Record::get_qr() const {
    return q->e[r % 4];
}

Edge_Record Edge_Record::rot() const {
    return q->rot(r);
}

Edge_Record Edge_Record::sym() const {
    return q->sym(r);
}

Edge_Record Edge_Record::rot_inv() const {
    return q->rot_inv(r);
}

Edge_Record Edge_Record::onext() const {
    return q->onext(r);
}

Edge_Record Edge_Record::oprev() const {
    return rot().onext().rot();
}

Edge_Record Edge_Record::lnext() const {
    return rot_inv().onext().rot();
}

Edge_Record Edge_Record::lprev() const {
    return onext().sym();
}

Edge_Record Edge_Record::rnext() const {
    return sym().onext();
}

Edge_Record Edge_Record::rprev() const {
    return rot().onext().rot_inv();
}

vertex Edge_Record::org() const {
    return get_qr().v;
}

vertex Edge_Record::dst() const {
    return sym().get_qr().v;
}

void Edge_Record::set_org(vertex p) {
    get_qr().v = p;
}

void Edge_Record::set_dst(vertex p) {
    sym().get_qr().v = p;
}

std::ostream& operator<<(std::ostream& strm, const Edge_Record& a) {
    return strm << "[" << a.org() << "," << a.dst() << "]";
}


// Q_Record
Q_Record::Q_Record() {
    data = NULL;
    flag = false;
}

Q_Record Q_Record::next() {
    return n_er.get_qr();
}

void Q_Record::set_next(Edge_Record er) {
    n_er = er;
}

// Quadedge
Quadedge::Quadedge() {
    for (int i = 0; i < 4; ++i) {
        e[i] = Q_Record();
        e[i].n_er = Edge_Record(this, i); // set .next of record to point to itself
    }
}

Quadedge::~Quadedge() {
    
}

Edge_Record Quadedge::rot(int r) {
    return Edge_Record(this, (r+1) % 4);
}

Edge_Record Quadedge::sym(int r) {
    return Edge_Record(this, (r+2) % 4);
}

Edge_Record Quadedge::rot_inv(int r) {
    return Edge_Record(this, (r+3) % 4);
}

Edge_Record Quadedge::onext(int r) {
    return e[r].n_er;
}


// basic topological operators

// the primal directed edge desired is e[0]
Edge_Record make_edge() {
    Quadedge* e = new Quadedge();
    e->e[1].set_next(Edge_Record(e, 3));
    e->e[3].set_next(Edge_Record(e, 1));
    return Edge_Record(e, 0);
}

// splice the edges with orgs a.get_qr() and b.get_qr()
void splice(Edge_Record a, Edge_Record b) {
    Edge_Record alpha, beta, a_onext, b_onext, alpha_onext, beta_onext;

    alpha = a.onext().rot();
    beta = b.onext().rot();

    // swap a.onext and b.onext
    a_onext = a.onext();
    b_onext = b.onext();
    a.get_qr().set_next(b_onext);
    b.get_qr().set_next(a_onext);

    // swap alpha.onext and beta.onext
    alpha_onext = alpha.onext();
    beta_onext = beta.onext();
    alpha.get_qr().set_next(beta_onext);
    beta.get_qr().set_next(alpha_onext);
}

// derived topological operators

// connects the a.dst to b.org through a new edge
edge connect(edge a, edge b) {
    Edge_Record er = make_edge();
    er.get_qr().v = a.dst();
    er.sym().get_qr().v = b.org();
    splice(er, a.lnext());
    splice(er.sym(), b);
    return er;
}

// removes an edge er, deletes its corresponding quadedge.
void delete_edge(edge er) {
    splice(er, er.oprev());
    splice(er.sym(), er.sym().oprev());
    delete er.q;
}

// swaps an edge (a.org, b.org) with (a.dst, b.dst)
void swap(edge er) {
    Edge_Record a, b;
    a = er.oprev();
    b = er.sym().oprev();
    splice(er, a);
    splice(er.sym(), b);
    splice(er, a.lnext());
    splice(er.sym(), b.lnext());
    er.get_qr().v = a.sym().get_qr().v;
    er.sym().get_qr().v = b.sym().get_qr().v;
}

edge make_edge(vertex org, vertex dst) {
    Edge_Record er = make_edge();
    er.get_qr().v = org;
    er.sym().get_qr().v = dst;
    return er;
}

void delete_all_edges(edge hull_edge) {
    edge e, e1, e2;
    std::vector<Quadedge*> *to_delete = new std::vector<Quadedge*>;

    // mark all hull edges as visited 
    e = hull_edge.sym();
    while (not e.get_qr().flag) {
        e.get_qr().flag = true; // visited
        to_delete->push_back(e.q);
        e = e.lnext();
    }

    // perform dfs and mark all visited faces
    std::stack<edge> fringe = std::stack<edge>();
    fringe.push(hull_edge);

    // invariants:
    // if e is popped, e's sym() was already visited
    // if e is not visited, neither have any of the other edges of the face
    while (not fringe.empty()) {
        e = fringe.top();
        fringe.pop();
        if (not e.get_qr().flag) {
            // visit the triangle left of e, mark e and all adjoining edges
            // add the sym() of each visited edge except e
            e1 = e.lnext();
            e2 = e1.lnext();
            // assert e == e2.lnext()
            e.get_qr().flag = true;
            if (not e.sym().get_qr().flag) {
                to_delete->push_back(e.q);
            }
            e1.get_qr().flag = true;
            if (not e1.sym().get_qr().flag) {
                to_delete->push_back(e1.q);
                fringe.push(e1.sym());
            }
            e2.get_qr().flag = true;
            if (not e2.sym().get_qr().flag) {
                to_delete->push_back(e2.q);
                fringe.push(e2.sym());
            }
        }
    }

    std::vector<Quadedge*>::iterator it;
    for (it = to_delete->begin(); it != to_delete->end(); ++it) {
        delete *it;
    }

    delete to_delete;
}

// given a counterclockwise convex hull edge, returns a string of triangles
// <v1> <v2> <v3>
// oriented counter clockwise, separated by newlines
std::string serialize_triangles(edge hull_edge) {
    edge e, e1, e2;

    // mark all hull edges as visited (we don't return the open face)
    e = hull_edge.sym();
    while (not e.get_qr().flag) {
        e.get_qr().flag = true; // visited
        e = e.lnext();
    }

    // perform dfs and mark all visited faces
    std::string output = "";
    std::stack<edge> fringe = std::stack<edge>();
    fringe.push(hull_edge);

    // invariants:
    // if e is popped, e's sym() was already visited
    // if e is not visited, neither have any of the other edges of the face
    while (not fringe.empty()) {
        e = fringe.top();
        fringe.pop();
        if (not e.get_qr().flag) {
            // visit the triangle left of e, mark e and all adjoining edges
            // add the sym() of each visited edge except e
            std::stringstream ss;
            e1 = e.lnext();
            e2 = e1.lnext();
            // assert e == e2.lnext()
            ss << e.org() << " ";
            e.get_qr().flag = true;
            ss << e1.org() << " ";
            e1.get_qr().flag = true;
            fringe.push(e1.sym());
            ss << e2.org() << "\n";
            e2.get_qr().flag = true;
            fringe.push(e2.sym());

            std::string triangle = ss.str();
            output += triangle;
        }
    }

    // unvisit the hull
    e = hull_edge.sym();
    while (e.get_qr().flag) {
        e.get_qr().flag = false; // unvisit
        e = e.lnext();
    }

    // perform dfs and unmark all visited faces
    fringe.push(hull_edge);

    // invariants:
    // if e is popped, e's sym() was already unvisited
    // if e is not visited, neither have any of the other edges of the face
    while (not fringe.empty()) {
        e = fringe.top();
        fringe.pop();
        if (e.get_qr().flag) {
            // unvisit the triangle left of e, unmark e and all adjoining edges
            // add the sym() of each unvisited edge except e
            e1 = e.lnext();
            e2 = e1.lnext();
            // assert e == e2.lnext()
            e.get_qr().flag = false;
            e1.get_qr().flag = false;
            fringe.push(e1.sym());
            e2.get_qr().flag = false;
            fringe.push(e2.sym());
        }
    }

    return output;
}

