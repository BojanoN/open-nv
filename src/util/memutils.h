#pragma once

#include <malloc.h>

#define MALLOC_WARN(type, retval)                       \
    type* retval = (type*)malloc(sizeof(type));         \
    if (retval == NULL) {                               \
        log_fatal("Cannot allocate memory for " #type); \
        return NULL;                                    \
    }
