#ifndef DA_H_
#define DA_H_

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define DA_INIT_CAP 1024
#define DA_GROW_RATE 1.5

#define DA(T) \
    struct { \
        size_t count; \
        size_t capacity; \
        T *items; \
    }

#define DA_TYPE(T) \
    typedef struct { \
        size_t count; \
        size_t capacity; \
        T *items; \
    }

#define da_free(da) \
    do { \
        free((da)->items); \
    } while(0)

#define da_reserve(da, expected) \
    do { \
        if((expected) > (da)->capacity) \
        { \
            if((da)->capacity == 0) (da)->capacity = DA_INIT_CAP; \
            while((expected) > (da)->capacity) (da)->capacity *= DA_GROW_RATE; \
            (da)->items = realloc((da)->items, sizeof(*(da)->items) * (da)->capacity); \
            assert((da)->items); \
        } \
    } while(0)

#define da_append(da, item) \
    do { \
        da_reserve(da, (da)->count + 1); \
        (da)->items[(da)->count++] = (item); \
    } while(0)

#define da_insert(da, item, i) \
    do { \
        assert((i) < (da)->count); \
        da_reserve(da, (da)->count + 1); \
        memmove((da)->items + (i) + 1, (da)->items + (i), sizeof(*(da)->items) * ((da)->count - (i))); \
        (da)->items[i] = (item); \
        (da)->count++; \
    } while(0)

#define da_first(da) (assert((da)->count > 0), (da)->items[0])

#define da_last(da) (assert((da)->count > 0), (da)->items[(da)->count - 1])

#define da_get(da, i) (assert((i) < (da)->count), (da)->items[i])

#define da_swap(da, i, j) \
    do { \
        typeof(*(da)->items) tmp = (da)->items[i]; \
        (da)->items[i] = (da)->items[j]; \
        (da)->items[j] = tmp; \
    } while(0)

#define da_remove_swap(da, i) \
    do { \
        assert((i) < (da)->count); \
        (da)->count--; \
        da_swap(da, i, (da)->count); \
    } while(0)

#define da_remove_del(da, i) \
    do { \
        assert((i) < (da)->count); \
        (da)->count--; \
        (da)->items[i] = (da)->items[(da)->count]; \
    } while(0)

#define da_sort(da, cmp) qsort((da)->items, (da)->count, sizeof(*(da)->items), cmp)

#define da_foreach(name, da) \
    for(typeof((da)->items) name = (da)->items; \
            name < (da)->items + (da)->count; \
            ++name)

#define da_foreach_reverse(name, da) \
    for(typeof((da)->items) name = (da)->count ? ((da)->items + (da)->count - 1) : NULL; \
            (da)->count && name >= (da)->items; \
            --name)

#endif // DA_H_
