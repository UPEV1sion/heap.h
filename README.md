# Heap in C

A Generic, typesafe[^1], header-only binary heap in C, with a nice API.
This heap implementation can serve both as a max and a min heap, and can be used as a priority queue.

[^1]: with respect to C type convertions

## Usage

```c
#include <stdio.h>

#define HEAP_IMPLEMENTATION
#include "heap.h"

int main(void)
{
    Heap(char *) heap = {
        .cmp = heap_cstr_asc
    };

    heap_insert(&heap, "lorem");
    heap_insert(&heap, "ipsum");
    heap_insert(&heap, "dolor");

    while(heap_size(&heap) > 0)
    {
        char *val; heap_pop(&heap, &val);
        puts(val);
    }

    heap_free(&heap);

    return 0;
}
```

## Public API

```text
heap_insert(heap_ptr, item)   insert an item into the heap
heap_pop(heap_ptr, out_ptr)   pop the top item of the heap
heap_size(heap_ptr)           get the number of elements in the heap
heap_clear(heap_ptr)          remove all elements from the heap
heap_free(heap_ptr)           free the heap memory
```

The library also provides convenience comparators for common types:

```c
int heap_int_desc(const void *a, const void *b);
int heap_int_asc(const void *a, const void *b);
int heap_cstr_desc(const void *a, const void *b);
int heap_cstr_asc(const void *a, const void *b);
```

By default `heap_int_desc` is used as a comparator
