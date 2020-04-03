#pragma once

#include "esm.h"
#include "subrecords.h"


typedef struct {
    Record base;
    HEDR*   hedr;
    CNAM*   cnam;
} TES4Record;
