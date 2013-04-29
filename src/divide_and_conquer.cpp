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

    if (left < right) {
        pivot_index = left + (rd() % (right - left + 1));
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
// even sized arrays, picks the last element of the lower partition
size_t lexico_partition(vertex v[], size_t n, bool by_x, vertex_buffer p) {
    size_t expected_med, random_index, part_index, offset;
    std::random_device rd;
    
    expected_med = (n-1)/2;
    offset = 0;
    while(true) {
        random_index = rd() % n;
        part_index = partition(v, 0, n-1, random_index, by_x, p);
        if (part_index + offset < expected_med) {
            // the partition we chose was below the median, recurse on upper half
            offset += part_index + 1;
            v += part_index + 1;
            n = n - (part_index + 1);
            continue;
        } else if (part_index + offset > expected_med) {
            // the partition ew chose was above the median, recurse on lower half
            n = part_index;
            continue;
        } else {
            return part_index + offset;
        }
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
        } else if (p.orient2d(v[0], v[2], v[1]) > 0) {
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

        ldo_ldi = delaunay_dc(v_l, v_l_n, p);
        ldo = ldo_ldi[0];
        ldi = ldo_ldi[1];
        rdi_rdo = delaunay_dc(v_r, v_r_n, p);
        rdi = rdi_rdo[0];
        rdo = rdi_rdo[1];

        // loop to compute lower common tangent of l and r
        while (true) {
            if (p.leftof(rdi.org(), ldi.org(), ldi.dst())) {
                ldi = ldi.lnext();
            } else if (p.rightof(ldi.org(), rdi.org(), rdi.dst())) {
                //rdi = rdi.rprev(); // Guibas and Stolfi wrong here?
                rdi = rdi.rnext();
            } else {
                break;
            }
        }

        // create the first cross edge basel from rdi.org to ldi.org
        basel = connect(rdi.sym(), ldi);
        if (ldi.org() == ldo.org()) {
            ldo = basel.sym();
        }
        if (rdi.org() == rdo.org()) {
            rdo = basel;
        }

        // merge loop
        while (true) {
            // is lcand valid?
            lcand = basel.sym().onext();
            lcand_valid = p.rightof(lcand.dst(), basel.org(), basel.dst());
            if (lcand_valid) {
                while (p.incircle(basel.dst(), basel.org(), lcand.dst(), lcand.onext().dst()) > 0) {
                    temp = lcand.onext();
                    delete_edge(lcand);
                    lcand = temp;
                }
            }
            // is rcand valid?
            rcand = basel.oprev();
            rcand_valid = p.rightof(rcand.dst(), basel.org(), basel.dst());
            if (rcand_valid) {
                while (p.incircle(basel.dst(), basel.org(), rcand.dst(), rcand.oprev().dst()) > 0) {
                    temp = rcand.oprev();
                    delete_edge(rcand);
                    rcand = temp;
                }
            }

            lcand_valid = p.rightof(lcand.dst(), basel.org(), basel.dst());
            rcand_valid = p.rightof(rcand.dst(), basel.org(), basel.dst());
            // if both lcand and rcand are invalid, 
            // then basel is the upper common tangent
            // ... break out of merge loop
            if ((not lcand_valid) and (not rcand_valid)) {
                break;
            }

            // the next cross edge is to be connected to either 
            // lcand.dst() or rcand.dst() 
            // if both valid, choose the appropriate one using incircle test
            if ((not lcand_valid) or ((rcand_valid and 
                (p.incircle(lcand.dst(), lcand.org(), rcand.org(), rcand.dst())) > 0))) {
                // add cross edge basel from rcand.dst() to basel.dst()
                basel = connect(rcand, basel.sym());
            } else {
                // add cross edge basel from basel.org() to lcand.dst()
                basel = connect(basel.sym(), lcand.sym());
            }
        }

        return edge_pair(ldo, rdo);
    }
}

edge_pair delaunay_dc2(vertex v[], size_t n, bool div_by_x, vertex_buffer p) {
    edge_pair ldo_ldi, rdi_rdo;
    edge a, b, c, ldo, ldi, rdo, rdi, basel, lcand, rcand, temp;
    vertex *v_l, *v_r;
    size_t median, v_l_n, v_r_n;
    bool lcand_valid, rcand_valid;

    if (n == 2) { 
        // for two vertices, return the two directed edges between them
        if (point_lte(p.val(v[0]), p.val(v[1]), div_by_x)) {
            a = make_edge(v[0], v[1]);
        } else {
            a = make_edge(v[1], v[0]);
        }
        return edge_pair(a, a.sym());

    } else if (n == 3) {
        // for three vertices
        a = make_edge();
        b = make_edge();
        splice(a.sym(), b);

        // order the vertices on the right axis
        bool zero_lte_one = point_lte(p.val(v[0]), p.val(v[1]), div_by_x);
        bool one_lte_two = point_lte(p.val(v[1]), p.val(v[2]), div_by_x);
        bool two_lte_zero = point_lte(p.val(v[2]), p.val(v[0]), div_by_x);
        vertex first, second, third;
        if (zero_lte_one) {
            if (one_lte_two) {
                first = v[0]; 
                second = v[1]; 
                third = v[2];
            } else {
                if (two_lte_zero) {
                    first = v[2];
                    second = v[0];
                    third = v[1];
                } else {
                    first = v[0];
                    second = v[2];
                    third = v[1];
                }
            }
        } else {
            if (one_lte_two) {
                if (two_lte_zero) {
                    first = v[1];
                    second = v[2];
                    third = v[0];
                } else {
                    first = v[1];
                    second = v[0];
                    third = v[2];
                }
            } else {
                first = v[2];
                second = v[1];
                third = v[0];
            }
        }
        a.set_org(first);
        a.set_dst(second);
        b.set_org(second);
        b.set_dst(third);
        if (p.orient2d(first, second, third) > 0) {
            c = connect(b, a);
            return edge_pair(a, b.sym());
        } else if (p.orient2d(first, third, second) > 0) {
            c = connect(b, a);
            return edge_pair(c.sym(), c);
        } else { // the points are collinear
            return edge_pair(a, b.sym());
        }

    } else { // n >= 4
        // div_by_x tells whether this recursive case is splitting by x
        // divide v into left half v_l, right half v_r
        median = lexico_partition(v, n, div_by_x, p);
        v_l = v;
        v_l_n = median + 1; // +1 ensures that the lower partition has >=2
        v_r = (v + v_l_n);
        v_r_n = (n - v_l_n);


        // recursive calls split in opposite axis; *do and *di are flipped!
        // must merge on the current axis, however
        ldo_ldi = delaunay_dc2(v_l, v_l_n, !div_by_x, p);
        ldo = ldo_ldi[0]; // ccw, so right face is open
        ldi = ldo_ldi[1]; // cw, so left face is open
        rdi_rdo = delaunay_dc2(v_r, v_r_n, !div_by_x, p);
        rdi = rdi_rdo[0]; // ccw, so right face is open
        rdo = rdi_rdo[1]; // cw, so left face is open

        // move *di, *do into the correct "leftmost", "rightmost" for level
        if (div_by_x) {
            while (point_lte(p.val(ldo.rprev().org()), p.val(ldo.org()), 
                    div_by_x)) {
                ldo = ldo.rprev();
            }
            while (point_lte(p.val(ldi.org()), p.val(ldi.lnext().org()), 
                    div_by_x)) {
                ldi = ldi.lnext();
            }
            while (point_lte(p.val(rdi.rprev().org()), p.val(rdi.org()), 
                    div_by_x)) {
                rdi = rdi.rprev();
            }
            while (point_lte(p.val(rdo.org()), p.val(rdo.lnext().org()), 
                    div_by_x)) {
                rdo = rdo.lnext();
            }
        } else {
            while (point_lte(p.val(ldo.rnext().org()), p.val(ldo.org()), 
                    div_by_x)) {
                ldo = ldo.rnext();
            }
            while (point_lte(p.val(ldi.org()), p.val(ldi.lprev().org()), 
                    div_by_x)) {
                ldi = ldi.lprev();
            }
            while (point_lte(p.val(rdi.rnext().org()), p.val(rdi.org()), 
                    div_by_x)) {
                rdi = rdi.rnext();
            }
            while (point_lte(p.val(rdo.org()), p.val(rdo.lprev().org()), 
                    div_by_x)) {
                rdo = rdo.lprev();
            }
        }

        // loop to compute lower common tangent of l and r
        while (true) {
            if (p.leftof(rdi.org(), ldi.org(), ldi.dst())) {
                ldi = ldi.lnext();
            } else if (p.rightof(ldi.org(), rdi.org(), rdi.dst())) {
                //rdi = rdi.rprev(); // Guibas and Stolfi wrong here?
                rdi = rdi.rnext();
            } else {
                break;
            }
        }

        // create the first cross edge basel from rdi.org to ldi.org
        basel = connect(rdi.sym(), ldi);
        if (ldi.org() == ldo.org()) {
            ldo = basel.sym();
        }
        if (rdi.org() == rdo.org()) {
            rdo = basel;
        }

        // merge loop
        while (true) {
            // is lcand valid?
            lcand = basel.sym().onext();
            lcand_valid = p.rightof(lcand.dst(), basel.org(), basel.dst());
            if (lcand_valid) {
                while (p.incircle(basel.dst(), basel.org(), lcand.dst(), lcand.onext().dst()) > 0) {
                    temp = lcand.onext();
                    delete_edge(lcand);
                    lcand = temp;
                }
            }
            // is rcand valid?
            rcand = basel.oprev();
            rcand_valid = p.rightof(rcand.dst(), basel.org(), basel.dst());
            if (rcand_valid) {
                while (p.incircle(basel.dst(), basel.org(), rcand.dst(), rcand.oprev().dst()) > 0) {
                    temp = rcand.oprev();
                    delete_edge(rcand);
                    rcand = temp;
                }
            }
            lcand_valid = p.rightof(lcand.dst(), basel.org(), basel.dst());
            rcand_valid = p.rightof(rcand.dst(), basel.org(), basel.dst());
            // if both lcand and rcand are invalid, 
            // then basel is the upper common tangent
            // ... break out of merge loop
            if ((not lcand_valid) and (not rcand_valid)) {
                break;
            }

            // the next cross edge is to be connected to either 
            // lcand.dst() or rcand.dst() 
            // if both valid, choose the appropriate one using incircle test
            if ((not lcand_valid) or ((rcand_valid and 
                (p.incircle(lcand.dst(), lcand.org(), rcand.org(), rcand.dst())) > 0))) {
                // add cross edge basel from rcand.dst() to basel.dst()
                basel = connect(rcand, basel.sym());
            } else {
                // add cross edge basel from basel.org() to lcand.dst()
                basel = connect(basel.sym(), lcand.sym());
            }
        }

        return edge_pair(ldo, rdo);
    }
}

