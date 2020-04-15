#pragma once
#include "types.hpp"

typedef struct __attribute__((packed)) {
    ESMName type;
    uint16_t dataSize;
} SubrecordHeader;
