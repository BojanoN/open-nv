#pragma once
#include "types.hpp"

typedef struct __attribute__((packed)) {
    char     Type[4];
    uint16_t DataSize;
} SubrecordHeader;
