#include "byte_hash.h"

static const uint32_t FNV_OFFSET = 2166136261;

uint32_t fnv_hash(const uint8_t* buffer, const uint32_t len)
{
    uint32_t sum = FNV_OFFSET;
    for (uint32_t i = 0; i < len; i++) {
        sum ^= buffer[i];
        sum += (sum << 1) + (sum << 4) + (sum << 7) + (sum << 8) + (sum << 24);
    }
    return sum;
}
