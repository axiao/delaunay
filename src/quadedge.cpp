#include "quadedge.h"

// Edge_Record
Edge_Record::Edge_Record() {

}

Edge_Record::Edge_Record(Quadedge* qe, int rot): 
    q(qe), r(rot) { };

Q_Record& Edge_Record::get_qr() {
    return q->e[r % 4];
}

Edge_Record Edge_Record::rot() {
    return q->rot(r);
}

Edge_Record Edge_Record::sym() {
    return q->sym(r);
}

Edge_Record Edge_Record::rot_inv() {
    return q->rot_inv(r);
}

Edge_Record Edge_Record::onext() {
    return q->onext(r);
}

vertex Edge_Record::org() {
    return get_qr().v;
}

vertex Edge_Record::dst() {
    return sym().get_qr().v;
}


// Q_Record
Q_Record::Q_Record() {

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
void splice(Edge_Record a_er, Edge_Record b_er) {
    Q_Record a_org, a_lp, b_org, b_lp; 
    Edge_Record a_ner, a_ler, b_ner, b_ler;

    // handle the org rings (primal)
    a_org = a_er.get_qr();
    b_org = b_er.get_qr();
    a_ner = a_er.onext();
    b_ner = b_er.onext();

    a_org.set_next(b_ner);
    b_org.set_next(a_ner);

    // handle the left rings (dual)
    a_ler = a_er.rot_inv();
    a_lp = a_ner.onext().get_qr();
    b_ler = b_er.rot_inv();
    b_lp = b_ner.onext().get_qr();

    a_lp.set_next(b_ler);
    b_lp.set_next(a_ler);
}

// derived topological operators

// connects the a.dst to b.org through a new edge
edge connect(edge a, edge b) {
    Edge_Record er = make_edge();
    er.get_qr().v = a.sym().get_qr().v;
    er.sym().get_qr().v = b.get_qr().v;
    splice(er, a.rot_inv().onext());
    splice(er.sym(), b);
    return er;
}

// removes an edge er, deletes its corresponding quadedge.
void delete_edge(edge er) {
    splice(er, er.rot().onext());
    splice(er.sym(), er.rot_inv().onext());
    delete er.q;
}

// swaps an edge (a.org, b.org) with (a.dst, b.dst)
void swap(edge er) {
    Edge_Record a, b;
    a = er.rot().onext();
    b = er.rot_inv().onext();
    splice(er, a);
    splice(er.sym(), b);
    splice(er, a.rot_inv().onext());
    splice(er.sym(), b.rot_inv().onext());
    er.get_qr().v = a.sym().get_qr().v;
    er.sym().get_qr().v = b.sym().get_qr().v;
}
