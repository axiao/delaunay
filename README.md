delaunay
========
A Delaunay triangulator.

Currently uses the quad-edge data structure for planar subdivisions described 
in a paper by Guibas and Stolfi, 
[Primitives for the Manipulation of General Subdivisions and the Computation of 
Voronoi Diagrams](http://portal.acm.org/citation.cfm?doid=282918.282923), 
ACM Transactions on Graphics 4(2):74–123, April 1985,
as well as the divide and conquer algorithm for computing Delaunay 
triangulations (described in the same paper).

build
--------
You can compile everything using `make` to create an executable called 
`delaunay`.

usage
-----
`delaunay` uses the [.node](http://www.cs.cmu.edu/~quake/triangle.node.html) 
and [.ele](http://www.cs.cmu.edu/~quake/triangle.ele.html) file formats from 
Shewchuk's [Triangle](http://www.cs.cmu.edu/~quake/triangle.html).

It takes in a file with suffix .node and outputs a similarly named file with
suffix .ele, in the formats described on the Triangle pages.
The actual triangulations (.ele) are viewable using the Show Me visualization 
program included with Triangle.

A common invocation will be:
> `~ delaunay foo.node`

Please use the --help option to see the other possible command line options.
> `~ delaunay -h`

credit
------
Almost all of the test cases are from a 
[project](http://www.cs.berkeley.edu/~jrs/274/proj.html) in Jonathan Shewchuk's 
[Computational Geometry](http://www.cs.berkeley.edu/~jrs/274/) 
class at UC Berkeley (CS274).
The project also uses his robust geometric 
[predicates](http://www.cs.cmu.edu/~quake/robust.html)
for floating point inputs (in particular Orient2D and InCircle).

