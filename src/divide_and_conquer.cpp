#include <algorithm>
#include <vector>
#include <iostream>

#include "divide_and_conquer.h"
#include "vertex.h"
#include "quadedge.h"
#include "geometric.h"


edge_pair::edge_pair() {

}

edge_pair::edge_pair(edge e0, edge e1):
    e0(e0), e1(e1) { }

edge edge_pair::operator[](size_t i) {
    if (i % 2 == 0) {
        return e0;
    } else {
        return e1;
    }
}


// outputs the edge_pair (le, re)
// le: counterclockwise convex hull edge out of the leftmost vertex
// re: clockwise convex hull edge out of the rightmost vertex
edge_pair delaunay_dc(vertex v[], size_t n, vertex_buffer p) {
    // assumes that v is sorted in lexicographic order
    edge_pair ldo_ldi, rdi_rdo;
    edge a, b, c, ldo, ldi, rdo, rdi, basel, lcand, rcand, temp;
    vertex *v_l, *v_r;
    size_t v_l_n, v_r_n;
    bool lcand_valid, rcand_valid;

    if (n == 2) { 
        // for two vertices, return the two directed edges between them
        a = make_edge(v[0], v[1]);
        return edge_pair(a, a.sym());

    } else if (n == 3) {
        // for three vertices
        a = make_edge();
        b = make_edge();
        splice(a.sym(), b);
        a.set_org(v[0]);
        a.set_dst(v[1]);
        b.set_org(v[1]);
        b.set_dst(v[2]);
        if (p.orient2d(v[0], v[1], v[2]) > 0) {
            c = connect(b, a);
            return edge_pair(a, b.sym());
        } else if (p.orient2d(v[0], v[2], v[1])) {
            c = connect(b, a);
            return edge_pair(c.sym(), c);
        } else { // the points are collinear
            return edge_pair(a, b.sym());
        }

    } else { // n >= 4
        // divide v into left half v_l, right half v_r
        v_l = v;
        v_l_n = n/2;
        v_r = (v + v_l_n);
        v_r_n = (n - v_l_n);

        ldo_ldi = delaunay_dc(v_l, v_l_n, p); // returns a size 2 array
        ldo = ldo_ldi[0];
        ldi = ldo_ldi[1];
        rdi_rdo = delaunay_dc(v_r, v_r_n, p); // returns a size 2 array
        rdi = rdi_rdo[0];
        rdo = rdi_rdo[1];

        std::cout << "ldo: " << ldo << " ldi: " << ldi << std::endl;
        std::cout << "rdi: " << rdi << " rdo: " << rdo << std::endl;

        // loop to compute lower common tangent of l and r
        while (true) {
            if (p.leftof(rdi.org(), ldi.org(), ldi.dst())) {
                std::cout << "swapped ldi for ldi.lnext" << std::endl;
                ldi = ldi.lnext();
            } else if (p.rightof(ldi.org(), rdi.org(), rdi.dst())) {
                std::cout << "swapped rdi for rdi.rprev" << std::endl;
                rdi = rdi.rprev();
            } else {
                break;
            }
        }

        std::cout << "computing lower common tangent" << std::endl;
        std::cout << "ldo: " << ldo << " ldi: " << ldi << std::endl;
        std::cout << "rdi: " << rdi << " rdo: " << rdo << std::endl;

        // create the first cross edge basel from rdi.org to ldi.org
        basel = connect(rdi.sym(), ldi);
        std::cout << "basel: " << basel << std::endl;
        if (ldi.org() == ldo.org()) {
            ldo = basel.sym();
            std::cout << "swapped ldo for basel.sym: " << ldo << std::endl;
        }
        if (rdi.org() == rdo.org()) {
            rdo = basel;
            std::cout << "swapped rdo for basel: " << rdo << std::endl;
        }
        std::cout << "basel's onext, oprev: org(" << basel.onext() << " " <<
            basel.oprev() << "), sym(" << basel.sym().onext() << " " <<
            basel.sym().oprev() << ")" << std::endl;;

        // merge loop
        while (true) {
            std::cout << "basel: " << basel << std::endl;
            // is lcand valid?
            lcand = basel.sym().onext();
            lcand_valid = p.rightof(lcand.dst(), basel.org(), basel.dst());
            std::cout << "lcand: " << lcand << std::endl;
            if (lcand_valid) {
                std::cout << "lcand valid: " << lcand << std::endl;
                while (p.incircle(basel.dst(), basel.org(), lcand.dst(), lcand.onext().dst()) > 0) {
                    temp = lcand.onext();
                    delete_edge(lcand);
                    lcand = temp;
                }
            }
            // is rcand valid?
            rcand = basel.oprev();
            rcand_valid = p.rightof(rcand.dst(), basel.org(), basel.dst());
            std::cout << "rcand: " << rcand << std::endl;
            if (rcand_valid) {
                std::cout << "rcand valid: " << rcand << std::endl;
                while (p.incircle(basel.dst(), basel.org(), rcand.dst(), rcand.oprev().dst()) > 0) {
                    temp = rcand.oprev();
                    delete_edge(rcand);
                    rcand = temp;
                }
            }
            // if both lcand and rcand are invalid, 
            // then basel is the upper common tangent
            // ... break out of merge loop
            if ((not lcand_valid) and (not rcand_valid)) {
                std::cout << "done here (basel is upper common tangent)" << std::endl;
                break;
            }

            // the next cross edge is to be connected to either 
            // lcand.dst() or rcand.dst() 
            // if both valid, choose the appropriate one using incircle test
            if ((not lcand_valid) or ((rcand_valid and 
                (p.incircle(lcand.dst(), lcand.org(), rcand.org(), rcand.dst())) > 0))) {
                // add cross edge basel from rcand.dst() to basel.dst()
                std::cout << "chose rcand " << rcand << std::endl;
                basel = connect(rcand, basel.sym());
            } else {
                // add cross edge basel from basel.org() to lcand.dst()
                std::cout << "chose lcand " << lcand << std::endl;
                basel = connect(basel.sym(), lcand.sym());
            }
        }
        std::cout << "final output edges" << std::endl;
        std::cout << "ldo: " << ldo << std::endl;
        std::cout << "rdo: " << rdo << std::endl;

        return edge_pair(ldo, rdo);
    }
}

edge_pair delaunay_dc2(vertex v[], size_t n, vertex_buffer p) {
    return edge_pair(); // TODO
}

// lexicographic quicksort
void lexico_sort(vertex v[], size_t n, vertex_buffer p) {
    quicksort(v, 0, n-1, p);
}

// helper method for in-place quicksort
size_t partition(vertex arr[], size_t left, size_t right, size_t piv, vertex_buffer p) {
    geometric::Point2 pivot_value;
    vertex temp;
    size_t stored, i;
    pivot_value = p.val(arr[piv]);
    // swap arr[piv] and arr[right]
    temp = arr[piv];
    arr[piv] = arr[right];
    arr[right] = temp;
    stored = left;
    for (i = left; i < right; ++i) {
        if (p.val(arr[i]) <= pivot_value) {
            temp = arr[i];
            arr[i] = arr[stored];
            arr[stored] = temp;
            ++stored;
        }
    }
    temp = arr[stored];
    arr[stored] = arr[right];
    arr[right] = temp;
    return stored;
}

void quicksort(vertex arr[], size_t left, size_t right, vertex_buffer p) {
    size_t pivot_index;
    if (left < right) {
        pivot_index = left + (std::rand() % (right - left + 1));
        pivot_index = partition(arr, left, right, pivot_index, p);
        quicksort(arr, left, pivot_index-1, p);
        quicksort(arr, pivot_index+1, right, p);
    }
}

// basically quickselect
int lexico_partition(vertex v[], size_t n, bool by_x, vertex_buffer p) {
 return 0; 
}
