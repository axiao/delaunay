#include "linkring.h"


// Links
Link::Link() {
    val = 0;
    prev = 0;
    next = 0;
}

Link::Link(const vertex& value) {
    val = value;
    prev = 0;
    next = 0;
}

Link::~Link() { }


// Linkrings
Linkring::Linkring() {
    empty = true;
    size = 0;
}

Linkring::~Linkring() {
    if (!empty) {
        Link* nxt;
        Link* it = star;
        do {
            nxt = it->next;
            delete it;
            it = nxt;
        } while (it != star);
    }
}

int Linkring::locate(const vertex& v) {
    // returns the index of v, or -1 if it can't find it
    if (empty) {
        return -1;
    }
    Link* target = star;
    int ret = 0;
    do {
        if (target->val == v) {
            return ret;
        }
        target = target->next; 
        ++ret;
    } while (target != star);
    return -1;
}

void Linkring::add_link(const vertex& v, int index) {
    // insert a Link for v before index
    if (empty) {
        star = new Link(v);
        empty = false;
    } else {
        Link* target = star;
        for (int i = 0; i < index; ++i) {
            target = target->next;
        } 
        target->prev->next = new Link(v);
        target->prev = target->prev->next;
    }
    ++size;
}

void Linkring::del_link(int index) {
    // delete the link at index
    Link* target = star;
    for (int i = 0; i < index; ++i) {
        target = target->next;
    }
    if ((target->next != 0) && (target->prev != 0)) {
        target->next->prev = target->prev;
        target->prev->next = target->next;
    }
    delete target;
    --size;
    if (size == 0) {
        empty = true;
    }
}
