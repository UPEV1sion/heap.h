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

// TODO think about this?
#define heap_pop(heap) \
    (typeof((heap)->items)) heap__pop((Heap__Abstract *) heap, layoutof(heap))

#define heap_free(heap) free((heap)->items)

void heap__insert(Heap__Abstract *heap, void *item, Heap__Layout *l);
void* heap__pop(Heap__Abstract *heap, Heap__Layout *l);

#endif // HEAP_H_

#ifdef HEAP_IMPLEMENTATION

#define HEAP_INIT_CAP 1024
#define HEAP_GROW_RATE 1.5

int heap__cmp_int(const void *a, const void *b)
{
    const int *ia = a;
    const int *ib = b;
    return (*ia > *ib) - (*ia < *ib);
}

#define heap__reserve(heap, expected, l) \
    do { \
        if((expected) > (heap)->capacity) \
        { \
            if((heap)->capacity == 0) (heap)->capacity = HEAP_INIT_CAP; \
            while((expected) > (heap)->capacity) (heap)->capacity *= HEAP_GROW_RATE; \
            (heap)->items = realloc((heap)->items, (l).item_size * (heap)->capacity); \
            assert((heap)->items); \
        } \
    } while(0)

#define heap__append(heap, item, l) \
    do { \
        heap__reserve(heap, (heap)->count + 1, l); \
        memmove(((uint8_t *) (heap)->items) + ((heap)->count * (l).item_size), (item), (l).item_size); \
        (heap)->count++; \
    } while(0)

#define heap__item_at(heap, i, l) \
    (((uint8_t *) (heap)->items) + ((i) * (l).item_size))

// TODO this is kinda dumb, but better than vlas 
#define heap__swap(heap, i, j, l) \
    do { \
        void *tmp = malloc(l.item_size); \
        memmove(tmp, heap__item_at(heap, i, l), l.item_size); \
        memmove(heap__item_at(heap, i, l), heap__item_at(heap, j, l), l.item_size); \
        memmove(heap__item_at(heap, j, l), tmp, l.item_size); \
        free(tmp); \
    } while(0)

void heap__heapify(Heap__Abstract *heap, size_t i, Heap__Layout *l)
{
    size_t largest = i;
    const size_t left = 2 * i + 1;
    const size_t right = 2 * i + 2;

    int (*cmp)(const void *, const void *) = heap->cmp ? heap->cmp : heap__cmp_int;
    if(left < heap->capacity && cmp(heap__item_at(heap, left, l), heap__item_at(heap, largest, l)) > 0)
    {
        largest = left;
    }

    if(right < heap->capacity && cmp(heap__item_at(heap, right, l), heap__item_at(heap, largest, l)) > 0)
    {
        largest = right;
    }

    if(largest != i)
    {
        heap__swap(heap, i, largest, l);
        heap__heapify(heap, largest);
    }
}

void heap__insert(Heap__Abstract *heap, void *item, Heap__Layout l)
{
    size_t i = heap->count;
    heap__append(heap, l, item);

    int (*cmp)(const void *, const void *) = heap->cmp ? heap->cmp : heap__cmp_int;
    while(i != 0 && cmp(heap__item_at(heap, (i - 1) / 2, l), heap__item_at(heap, i, l)) < 0)
    {
        const size_t parent = (i - 1) / 2;
        heap__swap(heap, i, parent, l);
        i = parent;
    }
}

void* heap__pop(Heap__Abstract *heap, Heap__Layout l)
{
    
}

#endif // HEAP_IMPLEMENTATION
