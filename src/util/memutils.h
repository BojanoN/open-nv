#pragma once

#include <malloc.h>

#define MALLOC_WARN(type, retval)                       \
    type* retval = (type*)calloc(1, sizeof(type));      \
    if (retval == NULL) {                               \
        log_fatal("Cannot allocate memory for " #type); \
        return NULL;                                    \
    }

#define MALLOC_N_WARN(type, n, retval)                  \
    type* retval = (type*)calloc((n), sizeof(type));    \
    if (retval == NULL) {                               \
        log_fatal("Cannot allocate memory for " #type); \
        return NULL;                                    \
    }
