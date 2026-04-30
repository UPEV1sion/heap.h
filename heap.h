#ifndef HEAP_H_
#define HEAP_H_

#ifdef NO_DA_H
#include "da.h"
#endif // NO_DA_H

#define Heap(T) \
    struct { \
        size_t count; \
        size_t capacity; \
        T *items; \
        int (*cmp)(const void *, const void *); \
    }



#endif // HEAP_H_
