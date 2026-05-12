#ifndef HEAP_H_
#define HEAP_H_

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

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
    (Heap__Layout) { \
        .item_size = sizeof(*(heap)->items), \
    }

#define heap_insert(heap, item) \
    heap__insert((Heap__Abstract *) heap, \
            (typeof(*(heap)->items)[]){item}, \
            layoutof(heap))

#define heap_pop(heap, out) \
    heap__pop((Heap__Abstract *) heap, \
            (out), \
            layoutof(heap))

#define heap_size(heap) (heap)->count

#define heap_clear(heap) (heap)->count = 0

#define heap_free(heap) free((heap)->items)

void heap__insert(Heap__Abstract *heap, void *item, Heap__Layout l);
void heap__pop(Heap__Abstract *heap, void *out, Heap__Layout l);

int heap_int_desc(const void *a, const void *b);
int heap_int_asc(const void *a, const void *b);
int heap_cstr_desc(const void *a, const void *b);
int heap_cstr_asc(const void *a, const void *b);

#endif // HEAP_H_

#ifdef HEAP_IMPLEMENTATION

#define HEAP_INIT_CAP 1024
#define HEAP_GROW_RATE 1.5

int heap_int_desc(const void *a, const void *b)
{
    const int *ia = a;
    const int *ib = b;
    return (*ia > *ib) - (*ia < *ib);
}

int heap_int_asc(const void *a, const void *b)
{
    const int *ia = a;
    const int *ib = b;
    return (*ia < *ib) - (*ia > *ib);
}

int heap_cstr_desc(const void *a, const void *b)
{
    const char * const*ca = a;
    const char * const*cb = b;
    return strcmp(*ca, *cb);
}

int heap_cstr_asc(const void *a, const void *b)
{
    const char * const*ca = a;
    const char * const*cb = b;
    return strcmp(*cb, *ca);
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

#define heap__swap(heap, i, j, l) \
    do { \
        uint8_t tmp[(l).item_size]; \
        memmove(tmp, heap__item_at(heap, i, l), l.item_size); \
        memmove(heap__item_at(heap, i, l), heap__item_at(heap, j, l), l.item_size); \
        memmove(heap__item_at(heap, j, l), tmp, l.item_size); \
    } while(0)

void heap__heapify(Heap__Abstract *heap, size_t i, Heap__Layout l)
{
    size_t largest = i;
    const size_t left = 2 * i + 1;
    const size_t right = 2 * i + 2;

    int (*cmp)(const void *, const void *) = heap->cmp ? heap->cmp : heap_int_desc;
    if(left < heap->count && cmp(heap__item_at(heap, left, l), heap__item_at(heap, largest, l)) > 0)
    {
        largest = left;
    }

    if(right < heap->count && cmp(heap__item_at(heap, right, l), heap__item_at(heap, largest, l)) > 0)
    {
        largest = right;
    }

    if(largest != i)
    {
        heap__swap(heap, i, largest, l);
        heap__heapify(heap, largest, l);
    }
}

void heap__insert(Heap__Abstract *heap, void *item, Heap__Layout l)
{
    size_t i = heap->count;
    heap__append(heap, item, l);

    int (*cmp)(const void *, const void *) = heap->cmp ? heap->cmp : heap_int_desc;
    while(i != 0 && cmp(heap__item_at(heap, (i - 1) / 2, l), heap__item_at(heap, i, l)) < 0)
    {
        const size_t parent = (i - 1) / 2;
        heap__swap(heap, i, parent, l);
        i = parent;
    }
}

void heap__pop(Heap__Abstract *heap, void *out, Heap__Layout l)
{
    if(heap->count <= 0) return;

    heap->count--;
    const size_t last = heap->count;
    memmove(out, heap->items, l.item_size);
    memmove(heap->items,
            ((uint8_t *)heap->items) + last * l.item_size,
            l.item_size);

    if(heap->count > 0) heap__heapify(heap, 0, l);
}

#endif // HEAP_IMPLEMENTATION


/*
    Revision history:

        2.0.0 (2026-05-12) Remove cmp from the comparators for a cleaner interface
        1.1.0 (2026-05-12) Add heap_clear
        1.0.0 (2026-05-04) First release
*/

/*

MIT License

Copyright (c) 2026 Emanuel Schäffer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
