#pragma once

#include <malloc.h>

#define SAFE_MALLOC(type, retval)                       \
    type* retval = (type*)malloc(sizeof(type));         \
    if (retval == NULL) {                               \
        log_fatal("Cannot allocate memory for " #type); \
        return NULL;                                    \
    }
