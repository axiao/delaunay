/**
 * Geometric types, predicates and utility functions
 */

#ifndef GEOMETRIC_H_
#define GEOMETRIC_H_

#include <math.h>
#include <iostream>

extern "C" {
#include "predicates.h"
};

namespace geometric {


// initializes the robust predicate library
// must be called before any predicates are used
inline void exactinit() {
    predicates::exactinit();
}

// simple 2d point class
class Point2 
{
    public:
        Point2() : x(0), y(0) { }
        Point2(double px, double py) : x(px), y(py) { }
        ~Point2() { }
        Point2& operator=(Point2 p) {
            x = p.x;
            y = p.y;
            return *this;
        }
        Point2 operator+(Point2& p) {
            return Point2(x += p.x, y += p.y);
        }
        Point2 operator-(Point2& p) {
            return Point2(x -= p.x, y -= p.y);
        }
        bool operator==(Point2& p) { 
            return (x == p.x) && (y == p.y);
        }
        bool operator!=(Point2& p) { 
            return (x != p.x) || (y != p.y);
        }
        // lexicographic comparison
        bool operator<(Point2& p) {
            if (x == p.x) {
                return y < p.y;
            }
            return x < p.x;
        }
        bool operator>(Point2& p) {
            if (x == p.x) {
                return y > p.y;
            }
            return x > p.x;
        }
        bool operator<=(Point2& p) {
            if (x == p.x) {
                return y <= p.y;
            }
            return x <= p.x;
        }
        bool operator>=(Point2& p) {
            if (x == p.x) {
                return y >= p.y;
            }
            return x >= p.x;
        }
        double * as_array() {
            return &x;
        }


        double x; // cartesian x coordinate
        double y; // cartesian y coordinate
};

inline std::ostream& operator<<(std::ostream& strm, Point2& a) {
    return strm << "(" << a.x << "," << a.y << ")";
}



// distance between 2 points in 2d
inline double distance(Point2& p1, Point2& p2) 
{
    double a = (p1.x - p2.x);
    double b = (p1.y - p2.y);
    return sqrt(a*a + b*b);
}

// abc are: ccw >0, colinear =0, cw <0
inline double orient2d(Point2& a, Point2& b, Point2& c)
{
    return predicates::orient2d(a.as_array(), b.as_array(), c.as_array());
/*
    return (a.x - c.x) * (b.y - c.y) - (b.x - c.x) * (a.y - c.y);
*/
}

// d is, with circle abc: inside >0, cocircular =0, outside <0  
inline double incircle(Point2& a, Point2& b, 
    Point2& c, Point2& d)
{
    return predicates::incircle(a.as_array(), b.as_array(), 
        c.as_array(), d.as_array());
/*
    double amx = a.x - d.x;
    double amy = a.y - d.y;
    double bmx = b.x - d.x;
    double bmy = b.y - d.y;
    double cmx = c.x - d.x;
    double cmy = c.y - d.y;
    double am2 = amx * amx + amy * amy;
    double bm2 = bmx * bmx + bmy * bmy;
    double cm2 = cmx * cmx + cmy * cmy;
    return amx*bmy*cm2 + cmx*amy*bm2 + bmx*cmy*am2 - 
           bmx*amy*cm2 - amx*cmy*bm2 - cmx*bmy*am2;
*/
}

// is point X left of the directed edge (org, dst)?
inline bool leftof(Point2& X, Point2& org, Point2& dst) {
    return orient2d(X, org, dst) > 0;
}

// is point X right of the directed edge (org, dst)?
inline bool rightof(Point2& X, Point2& org, Point2& dst) {
    return orient2d(X, dst, org) > 0;
}

}

#endif
