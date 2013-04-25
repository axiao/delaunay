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
        Triangulation();
        ~Triangulation();
        virtual void init(vertex_buffer*);
        // TODO triangulation expected methods
    private:
        vertex_buffer* v_buf;
};

#endif
