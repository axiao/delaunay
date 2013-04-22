/**
 * Main abstract triangulation class.
 *
 */

#ifndef TRIANGULATION_H_
#define TRIANGULATION_H_

#include "vertex.h"

class Triangulation
{
    public:
        virtual Triangulation() = 0;
        virtual ~Triangulation() = 0;
        virtual void init(V_BUFFER) = 0;
        // TODO triangulation expected methods
    private:
        V_BUFFER v_buf;
};

#endif
