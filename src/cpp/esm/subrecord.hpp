#pragma once
#include "types.hpp"

typedef struct __attribute__((packed)) {
    uint32_t Type;
    uint16_t DataSize;
} SubrecordHeader;
