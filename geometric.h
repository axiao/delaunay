/*
 * Geometric predicates and utility functions
 */

#include <math.h>

#ifndef GEOMETRIC_H_
#define GEOMETRIC_H_

/*
 * simple 2d point class
 */
class Point2 
{
    public:
        Point2() : x(0), y(0) { }
        Point2(double px, double py) : x(px), y(py) { }
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
        double x; // cartesian x coordinate
        double y; // cartesian y coordinate
};

/*
 * oriented edge in 2d, org->dst
 */
class Edge2
{
    public:
        Edge2() : org(Point2(0, 0)), dst(Point2(0, 0)) { }
        Edge2(Point2 o, Point2 d) : org(o), dst(d) { }
        ~Edge2() { }
        bool operator==(const Edge2& e) { 
            return (org == e.org) && (dst == e.dst);
        }
        bool operator!=(const Edge2& e) { 
            return (org != e.org) || (dst != e.dst);
        }
        Point2 org; // origin of oriented edge
        Point2 dst; // destination of oriented edge
};

/*
 * oriented triangle in 2d
 * positive orientation means that orient2d(u, v, w) returns >0
 */
class Triangle2
{
    public:
        Triangle2() : u(Point2(0, 0)), v(Point2(0, 0)), w(Point2(0, 0)) { }
        Triangle2(Point2 pu, Point2 pv, Point2 pw) : u(pu), v(pv), w(pw) { }
        ~Triangle2() { }
        Point2 u;
        Point2 v;
        Point2 w;
};



/*
 * distance between 2 points in 2d
 */
static double distance(const Point2& p1, const Point2& p2) 
{
    double a = (p1.x - p2.x);
    double b = (p1.y - p2.y);
    return sqrt(a*a + b*b);
}

/*
 * abc are: ccw >0, colinear =0, cw <0
 */
static double orient2d(const Point2& a, const Point2& b, const Point2& c)
{
    return (a.x - c.x) * (b.y - c.y) - (b.x - c.x) * (a.y - c.y);
}

/*
 * d is, with circle abc: inside >0, cocircular =0, outside <0  
 */
static double incircle(const Point2& a, const Point2& b, const Point2& c, 
                       const Point2& d)
{
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
}

#endif
