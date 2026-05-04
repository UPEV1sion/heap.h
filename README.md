# Heap in C

A Generic, typesafe[^1], header-only heap in C, with a nice API.

[^1]: with respect to the C type convertions

## Usage

```c
#include <stdio.h>

#define HEAP_IMPLEMENTATION
#include "heap.h"

int main(void)
{
    Heap(char *) heap = {
        .cmp = heap_cmp_cstr_asc
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
heap_size(heap_ptr)           get the remaining heap size
heap_free(heap_ptr)           free the heap memory
```

