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

#define heap_free(heap) free((heap)->items)

void heap__insert(Heap__Abstract *heap, void *item, Heap__Layout *l);
void* heap__pop(Heap__Abstract *heap, Heap__Layout *l);

#endif // HEAP_H_

#ifdef HEAP_IMPLEMENTATION

#define HEAP_INIT_CAP 1024
#define HEAP_GROW_RATE 1.5

#define heap__reserve(heap, layout, expected) \
    do { \
        if((expected) > (heap)->capacity) \
        { \
            if((heap)->capacity == 0) (heap)->capacity = HEAP_INIT_CAP; \
            while((expected) > (heap)->capacity) (heap)->capacity *= HEAP_GROW_RATE; \
            (heap)->items = realloc((heap)->items, (layout)->item_size * (heap)->capacity); \
            assert((heap)->items); \
        } \
    } while(0)

#define heap__append(heap, layout, item) \
    do { \
        heap__reserve(heap, layout, (heap)->count + 1); \
        memmove(((uint8_t *) (heap)->items) + (heap)->count, (item), (layout)->item_size); \
        (heap)->count++; \
    } while(0)

void heap__insert(Heap__Abstract *heap, void *item, Heap__Layout *l)
{
    heap__append(heap, l, item); 
    
}

void* heap__pop(Heap__Abstract *heap, Heap__Layout *l)
{

}

#endif // HEAP_IMPLEMENTATION
