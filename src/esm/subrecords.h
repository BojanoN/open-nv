#pragma once

#include "esm.h"

typedef struct __attribute__((packed)) {
    Subrecord base;
    float    version;
    uint32_t numRecords;
    uint32_t nextObjectId;
} HEDR;

typedef struct {
    Subrecord base;
    sds       author;
} CNAM;
