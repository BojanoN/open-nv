#pragma once
#include "types.hpp"

namespace ESM {
typedef struct __attribute__((packed)) {
    ESMType  type;
    uint16_t dataSize;
} SubrecordHeader;
}
