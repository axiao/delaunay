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
inline void init() {
    exactinit();
}

// simple 2d point class
class Point2 
{
    public:
        Point2() : x(0), y(0) { }
        Point2(long double px, double py) : x(px), y(py) { }
        ~Point2() { }
        Point2& operator=(const Point2& p) {
            x = p.x;
            y = p.y;
            return *this;
        }
        Point2 operator+(const Point2& p) {
            return Point2(x += p.x, y += p.y);
        }
        Point2 operator-(const Point2& p) {
            return Point2(x -= p.x, y -= p.y);
        }
        bool operator==(const Point2& p) { 
            return (x == p.x) && (y == p.y);
        }
        bool operator!=(const Point2& p) { 
            return (x != p.x) || (y != p.y);
        }
        // lexicographic comparison
        bool operator<(const Point2& p) {
            if (x == p.x) {
                return y < p.y;
            }
            return x < p.x;
        }
        bool operator>(const Point2& p) {
            if (x == p.x) {
                return y > p.y;
            }
            return x > p.x;
        }
        bool operator<=(const Point2& p) {
            if (x == p.x) {
                return y <= p.y;
            }
            return x <= p.x;
        }
        bool operator>=(const Point2& p) {
            if (x == p.x) {
                return y >= p.y;
            }
            return x >= p.x;
        }


        long double x; // cartesian x coordinate
        long double y; // cartesian y coordinate
};

inline std::ostream& operator<<(std::ostream& strm, const Point2& a) {
    return strm << "(" << a.x << "," << a.y << ")";
}



// distance between 2 points in 2d
inline long double distance(const Point2& p1, const Point2& p2) 
{
    long double a = (p1.x - p2.x);
    long double b = (p1.y - p2.y);
    return sqrt(a*a + b*b);
}

// abc are: ccw >0, colinear =0, cw <0
inline long double orient2d(const Point2& a, const Point2& b, const Point2& c)
{
    return (a.x - c.x) * (b.y - c.y) - (b.x - c.x) * (a.y - c.y);
}

// d is, with circle abc: inside >0, cocircular =0, outside <0  
inline long double incircle(const Point2& a, const Point2& b, const Point2& c, 
                       const Point2& d)
{
    long double amx = a.x - d.x;
    long double amy = a.y - d.y;
    long double bmx = b.x - d.x;
    long double bmy = b.y - d.y;
    long double cmx = c.x - d.x;
    long double cmy = c.y - d.y;
    long double am2 = amx * amx + amy * amy;
    long double bm2 = bmx * bmx + bmy * bmy;
    long double cm2 = cmx * cmx + cmy * cmy;
    return amx*bmy*cm2 + cmx*amy*bm2 + bmx*cmy*am2 - 
           bmx*amy*cm2 - amx*cmy*bm2 - cmx*bmy*am2;
}

// is point X left of the directed edge (org, dst)?
inline bool leftof(const Point2& X, const Point2& org, const Point2& dst) {
    return orient2d(X, org, dst) > 0;
}

// is point X right of the directed edge (org, dst)?
inline bool rightof(const Point2& X, const Point2& org, const Point2& dst) {
    return orient2d(X, dst, org) > 0;
}

}

#endif
