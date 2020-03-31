#pragma once
#include <stdint.h>

struct ref {
    void (*free)(const struct ref* obj);
    uint32_t count;
};

static inline void
ref_dec(const struct ref* obj)
{
    ((struct ref*)obj)->count--;
    if (obj->count == 0) {
        obj->free(obj);
    }
}

static inline void
ref_inc(const struct ref* obj)
{
    ((struct ref*)obj)->count++;
}
