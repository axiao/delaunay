/**
 * handle for predicate library
 */

#ifndef _PREDICATES_H_
#define _PREDICATES_H_

#ifdef __cplusplus // for C++ compiler, use C linkage
extern "C" {
#endif

namespace predicates {

void exactinit();
double orient2d(double*, double*, double*);
double incircle(double*, double*, double*, double*);

}

#ifdef __cplusplus // for C++ compiler, end C linkage
}
#endif

#endif
