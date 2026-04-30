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
typedef struct {
    size_t count;
    size_t capacity;
    void *items;
    int (*cmp)(const void *, const void *);
} Heap__Abstract;

typedef struct {
    size_t item_size;
} Heap__Layout;

#define layoutof(heap) \
    (Heap__Layout) struct { \
        .item_size = sizeof(*(heap)->items), \
    }

#define heap_insert(heap, item) \
    heap__insert((Heap__Abstract *) heap, \
            (typeof(*(heap)->items)[]{item}), \
            layoutof(heap))

#define heap_pop(heap) \
    (typeof((heap)->items)) heap__pop((Heap__Abstract *) heap, layoutof(heap))

void heap__insert(Heap__Abstract *heap, void *item, Heap__Layout *l);
void* heap__pop(Heap__Abstract *heap, Heap__Layout *l);

#endif // HEAP_H_

#ifdef HEAP_IMPLEMENTATION

void heap__insert(Heap__Abstract *heap, void *item, Heap__Layout *l)
{

}

void* heap__pop(Heap__Abstract *heap, Heap__Layout *l)
{

}

#endif // HEAP_IMPLEMENTATION
