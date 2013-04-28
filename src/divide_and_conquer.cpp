#include <random>
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
        std::cout << "***TWO VERTICES" << std::endl;
        a = make_edge(v[0], v[1]);
        std::cout << "***final output edges" << std::endl;
        std::cout << "***le: " << a << std::endl;
        std::cout << "***re: " << a.sym() << std::endl;
        return edge_pair(a, a.sym());

    } else if (n == 3) {
        // for three vertices
        std::cout << "***THREE VERTICES" << std::endl;
        a = make_edge();
        b = make_edge();
        splice(a.sym(), b);
        a.set_org(v[0]);
        a.set_dst(v[1]);
        b.set_org(v[1]);
        b.set_dst(v[2]);
        std::cout << "created edge " << a << std::endl;
        std::cout << "created edge " << b << std::endl;
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

        std::cout << "***FOUR+ VERTICES" << std::endl;

        std::cout << "ldo: " << ldo << " ldi: " << ldi << std::endl;
        std::cout << "rdi: " << rdi << " rdo: " << rdo << std::endl;

        // loop to compute lower common tangent of l and r
        while (true) {
            if (p.leftof(rdi.org(), ldi.org(), ldi.dst())) {
                std::cout << "swapped ldi for ldi.lnext" << std::endl;
                ldi = ldi.lnext();
            } else if (p.rightof(ldi.org(), rdi.org(), rdi.dst())) {
                std::cout << "swapped rdi for rdi.rnext" << std::endl;
                //rdi = rdi.rprev(); // Guibas and Stolfi wrong here?
                rdi = rdi.rnext();
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
            std::cout << "lcand: " << lcand << std::endl;
            lcand_valid = p.rightof(lcand.dst(), basel.org(), basel.dst());
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
            std::cout << "rcand: " << rcand << std::endl;
            rcand_valid = p.rightof(rcand.dst(), basel.org(), basel.dst());
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
        std::cout << "***final output edges" << std::endl;
        std::cout << "***le: " << ldo << std::endl;
        std::cout << "***re: " << rdo << std::endl;

        return edge_pair(ldo, rdo);
    }
}

edge_pair delaunay_dc2(vertex v[], size_t n, vertex_buffer p, bool div_by_x) {
    edge_pair ldo_ldi, rdi_rdo;
    edge a, b, c, ldo, ldi, rdo, rdi, basel, lcand, rcand, temp;
    vertex *v_l, *v_r;
    size_t v_l_n, v_r_n;
    bool lcand_valid, rcand_valid;

    if (n == 2) { 
        // for two vertices, return the two directed edges between them
        std::cout << "***TWO VERTICES" << std::endl;
        a = make_edge(v[0], v[1]);
        std::cout << "***final output edges" << std::endl;
        std::cout << "***le: " << a << std::endl;
        std::cout << "***re: " << a.sym() << std::endl;
        return edge_pair(a, a.sym());

    } else if (n == 3) {
        // for three vertices
        std::cout << "***THREE VERTICES" << std::endl;
        a = make_edge();
        b = make_edge();
        splice(a.sym(), b);
        a.set_org(v[0]);
        a.set_dst(v[1]);
        b.set_org(v[1]);
        b.set_dst(v[2]);
        std::cout << "created edge " << a << std::endl;
        std::cout << "created edge " << b << std::endl;
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
        // TODO change the recursive calls to alternate in x and y
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

        std::cout << "***FOUR+ VERTICES" << std::endl;

        std::cout << "ldo: " << ldo << " ldi: " << ldi << std::endl;
        std::cout << "rdi: " << rdi << " rdo: " << rdo << std::endl;

        // loop to compute lower common tangent of l and r
        while (true) {
            if (p.leftof(rdi.org(), ldi.org(), ldi.dst())) {
                std::cout << "swapped ldi for ldi.lnext" << std::endl;
                ldi = ldi.lnext();
            } else if (p.rightof(ldi.org(), rdi.org(), rdi.dst())) {
                std::cout << "swapped rdi for rdi.rnext" << std::endl;
                //rdi = rdi.rprev(); // Guibas and Stolfi wrong here?
                rdi = rdi.rnext();
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
            std::cout << "lcand: " << lcand << std::endl;
            lcand_valid = p.rightof(lcand.dst(), basel.org(), basel.dst());
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
            std::cout << "rcand: " << rcand << std::endl;
            rcand_valid = p.rightof(rcand.dst(), basel.org(), basel.dst());
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
        std::cout << "***final output edges" << std::endl;
        std::cout << "***le: " << ldo << std::endl;
        std::cout << "***re: " << rdo << std::endl;

        return edge_pair(ldo, rdo);
    }

    // TODO
    return edge_pair(); 
}


bool point_lte(geometric::Point2 p1, geometric::Point2 p2, bool by_x) {
    if (by_x) {
        if (p1.x == p2.x) {
            return p1.y <= p2.y;
        } else {
            return p1.x < p2.x;
        }
    } else { // order lexico y first
        if (p1.y == p2.y) {
            return p1.x <= p2.x;
        } else {
            return p1.y < p2.y;
        }
    }
}

// helper method for in-place quicksort
size_t partition(vertex arr[], size_t left, size_t right, size_t piv, bool by_x, vertex_buffer p) {
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
        //if (p.val(arr[i]) <= pivot_value) {
        if (point_lte(p.val(arr[i]), pivot_value, by_x)) {
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

// a quicksort into lexicographic order
void quicksort(vertex arr[], size_t left, size_t right, vertex_buffer p) {
    size_t pivot_index;
    std::random_device rd;

    std::cout << left << " " << right << std::endl;
    if (left < right) {
        pivot_index = left + (rd() % (right - left + 1));
        std::cout << pivot_index << std::endl;
        pivot_index = partition(arr, left, right, pivot_index, true, p);
        if (pivot_index > left) {
            quicksort(arr, left, pivot_index-1, p);
        }
        if (pivot_index < right) {
            quicksort(arr, pivot_index+1, right, p);
        }
    }
}

// lexicographic quicksort
void lexico_sort(vertex v[], size_t n, vertex_buffer p) {
    quicksort(v, 0, n-1, p);
}

// basically partitions v about its medial element in x or y coordinate 
size_t lexico_partition(vertex v[], size_t n, bool by_x, vertex_buffer p) {
    size_t expected_med, random_index, part_index, offset;
    std::random_device rd;
    
    std::cout << "number of vertices: " << n << std::endl;
    expected_med = (n-1)/2;
    offset = 0;
    std::cout << "expected median: " << expected_med << std::endl;
    std::cout << ">>> LOOPING" << std::endl;
    while(true) {
        std::cout << "number of vertices: " << n << std::endl;
        std::cout << "vertices: ";
        for (size_t i = 0; i < n; ++i) {
            std::cout << v[i] << " ";
        }
        std::cout << std::endl;
        random_index = rd() % n;
        std::cout << "random index: " << random_index << std::endl;
        part_index = partition(v, 0, n-1, random_index, by_x, p);
        std::cout << "partitioned index: " << part_index << std::endl;
        std::cout << "offset: " << offset << std::endl;
        if (part_index + offset < expected_med) {
            std::cout << "recurse to upper half portion" << std::endl;
            // the partition we chose was below the median, recurse on upper half
            offset += part_index + 1;
            v += part_index + 1;
            n = n - (part_index + 1);
            continue;
        } else if (part_index + offset > expected_med) {
            std::cout << "recurse to lower half portion" << std::endl;
            // the partition ew chose was above the median, recurse on lower half
            n = part_index;
            continue;
        } else {
            std::cout << "verdict: " << part_index << " with offset " << offset << std::endl;
            return part_index + offset;
        }
    }
}

